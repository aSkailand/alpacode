#include "Farmer.h"
#include "../alpaca/alpaca.h"
#include "../trap/Trap.h"

Farmer::Farmer(ConfigGame *configGame, float radius, float width, float height, float x, float y) {

    this->configGame = configGame;
    farmerSpriteMapPtr = configGame->farmerSprites;

    convertAngleToVectors((int) Action::WALKING, walkAngle);
    convertAngleToVectors((int) Action::JUMP, jumpAngle);
    convertAngleToVectors((int) Grasp::THROWING, throwAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = configGame->world->CreateBody(&bodyDef);

    // // Create Fixture_body
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_body;
    fixtureDef_body.density = density;
    fixtureDef_body.friction = friction;
    fixtureDef_body.restitution = restitution;
    fixtureDef_body.filter.categoryBits = categoryBits;
    fixtureDef_body.filter.maskBits = maskBits;
    fixtureDef_body.shape = &b2Shape;

    // Create fixtureDef_hit
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_hit;
    fixtureDef_hit.shape = &b2Shape2;
    fixtureDef_hit.isSensor = true;
    fixtureDef_hit.filter.categoryBits = (uint16) ID::FARMER;
    fixtureDef_hit.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::WOLF | (uint16) ID::SHOTGUN | (uint16) ID::TRAP;

    // Store information
    setEntity_ID(Entity::ID::FARMER);
    body->SetUserData((void *) this);

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef_body);
    fixture_hit = body->CreateFixture(&fixtureDef_hit);

    // Set Fixture Sense to given Enums
    fixture_body->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    fixture_hit->SetUserData(convertToVoidPtr((int) CollisionID::HIT));

    // Create SFML shape
    sf_ShapeEntity = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_ShapeEntity->setOrigin(width / 2, height / 2);

    sf_ShapeGhost = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_ShapeGhost->setOrigin(width / 2, height / 2);
    sf_ShapeGhost->setTexture(farmerSpriteMapPtr[Action::WALKING][2]);
    sf_ShapeGhost->setFillColor(sf::Color(200, 200, 200, 100));

    sf_DebugHit = new sf::CircleShape(radius);
    sf_DebugHit->setFillColor(sf::Color::Transparent);
    sf_DebugHit->setOrigin(radius, radius);

    // Set HP
    HP = max_HP;

    // Create HitPoint barometer
    hitPointBarometer = new HitPointBarometer(this->configGame, HP, 25.f, 25.f);

    // Create ID
    label_ID = configGame->createLabel(&configGame->fontID, 20, "Farmer");


    graspClock.reset(true);
    movementTriggerClock.reset(true);

}


void Farmer::render(sf::RenderWindow *window) {

    // Switch Texture
    switchCurrentTexture();

    calcShapeEntityPlacement();
    if(currentHealth != Health::ALIVE)
        sf_ShapeEntity->setRotation((getBody()->GetAngle() * DEGtoRAD + 90));

    if (currentHealth == Health::GHOST) {
        renderDeath();
        window->draw(*sf_ShapeGhost);
    }

    window->draw(*sf_ShapeEntity);

    // Draw Hit Points Barometer
    if(currentHealth == Health::ALIVE && !configGame->isPaused){
        hitPointBarometer->setPlacement(getBody()->GetWorldPoint(b2Vec2(0.f, -3.f)).x * SCALE,
                                        getBody()->GetWorldPoint(b2Vec2(0.f, -3.f)).y * SCALE,
                                        sf_ShapeEntity->getRotation());
        hitPointBarometer->render(window);
    }

    renderDebugMode();

}

void Farmer::switchAction() {

    // Check current health and update according to current health
    handleHealth();

    // Cancel if not alive
    if(currentHealth != Health::ALIVE){
        return;
    }

    // Handle Input
    switch (configGame->currentInput) {
        case sf::Keyboard::W: {
            currentAction = Action::JUMP;
            break;
        }
        case sf::Keyboard::D: {
            currentAction = Action::WALKING;
            currentDirection = Direction::RIGHT;
            break;
        }
        case sf::Keyboard::A: {
            currentAction = Action::WALKING;
            currentDirection = Direction::LEFT;
            break;
        }
        case sf::Keyboard::E: {
            if (isCooldownTriggered(&graspClock, graspCooldown)) {
                if (currentGrasp == Grasp::EMPTY) {
                    if (!currentlyTouchingEntities.empty()) {
                        currentGrasp = Grasp::REACHING;
                    }
                } else if (currentGrasp == Grasp::HOLDING) {
                    currentGrasp = Grasp::THROWING;
                }
            }
            break;
        }
        default: {
            currentAction = Action::IDLE;
            break;
        }
    }

    // Flip accordingly to mouse placement
    sf_ShapeEntity->setScale(configGame->mouseInLeftSide ? -1.f : 1.f, 1.f);

}

void Farmer::performAction() {

    // If alive and allowed to move
    if (currentHealth == Health::ALIVE
        && currentAction != Action::IDLE
        && currentStatus == Status::GROUNDED
        && isMovementAvailable(moveAvailableTick)) {

        switch (currentAction) {

            case Action::WALKING: {
                forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                break;
            }
            case Action::JUMP: {
                if (configGame->mouseInLeftSide) {
                    forcePushBody((int) Action::JUMP, getBody(), jumpForce, Direction::LEFT);
                } else {
                    forcePushBody((int) Action::JUMP, getBody(), jumpForce, Direction::RIGHT);
                }
                break;
            }
            case Action::IDLE: {
                break;
            }

        }
    }

    // Throw held entity if not alive
    if(currentHealth != Health::ALIVE){
        if(holdingEntity != nullptr){
            currentGrasp = Grasp::THROWING;
        }
    }

    // Check the current status of grasp
    switch (currentGrasp) {
        case Grasp::REACHING: {

            if (holdingEntity == nullptr) {

                holdingEntity = currentlyTouchingEntities.front();

                dynamic_cast<Holdable *>(holdingEntity)->isHeld = true;
                dynamic_cast<Holdable *>(holdingEntity)->performHold();

                auto *warm = dynamic_cast<EntityWarm *> (holdingEntity);
                if (warm) {
                    warm->currentStatus = Status::AIRBORNE;
                }

                currentGrasp = Grasp::HOLDING;

                graspClock.reset(true);

            }
            break;

        }

        case Grasp::HOLDING: {

            switch (holdingEntity->getEntity_ID()) {
                case ID::ALPACA: {
                    b2Vec2 offset = getBody()->GetWorldPoint(b2Vec2(0.f, -3.f));
                    holdingEntity->getBody()->SetTransform(offset, getBody()->GetAngle());
                    break;
                }
                case ID::SHOTGUN: {

                    b2Vec2 mousePosition = b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
                    b2Vec2 toTarget = getBody()->GetWorldCenter() - mousePosition;
                    float angle = atan2(-toTarget.x, toTarget.y) - 90.f / DEGtoRAD;

                    b2Vec2 offset = getBody()->GetWorldPoint(b2Vec2(0.f, 0.f));
                    holdingEntity->getBody()->SetTransform(offset, angle);

                    break;
                }
                case ID::TRAP: {
                    b2Vec2 offset = getBody()->GetWorldPoint(b2Vec2(0.f, -3.f));
                    holdingEntity->getBody()->SetTransform(offset, getBody()->GetAngle());
                    break;
                }
                default: {
                    break;
                }
            }

            break;

        }

        case Grasp::THROWING: {

            dynamic_cast<Holdable*>(holdingEntity)->isHeld = false;
            dynamic_cast<Holdable*>(holdingEntity)->performThrow();

            // Reset Rotation (So that throwing angle works as intended)
            holdingEntity->getBody()->SetTransform(holdingEntity->getBody()->GetWorldCenter(), getBody()->GetAngle());

            // Reset Speed
            holdingEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));

            // Get mouse angle
            b2Vec2 mousePosition = b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
            b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() - mousePosition;
            toTarget.Normalize();

            float mass = holdingEntity->getBody()->GetMass();

            // Push body
            holdingEntity->getBody()->ApplyLinearImpulseToCenter(throwForce * mass * -toTarget, true);

            // Reset variables
            holdingEntity = nullptr;
            currentGrasp = Grasp::EMPTY;

            break;
        }

        case Grasp::EMPTY: {
            break;
        }

    }

}

void Farmer::endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {

    switch (selfCollision) {
        case CollisionID::BODY: {
            endContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::HIT: {
            endContact_hit(otherCollision, contactEntity);
            break;
        }
        case CollisionID::DETECTION:
            break;
    }
}


void Farmer::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (selfCollision) {
        case CollisionID::BODY: {
            startContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::HIT: {
            startContact_hit(otherCollision, contactEntity);
            break;
        }
        case CollisionID::DETECTION:
            break;
    }
}

bool Farmer::deadCheck() {
    return currentHealth == Health::GHOST && !deathClock.isRunning();
}

bool Farmer::checkIfTouching(Entity *entity) {
    return std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(),
                     entity) != currentlyTouchingEntities.end();
}

void Farmer::startContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            spriteSwitch = !spriteSwitch;
            sf_DebugHit->setOutlineColor(sf::Color::White);
            break;
        }
        default:
            break;
    }
}

void Farmer::startContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::ALPACA: {
            if (otherCollision == CollisionID::HIT && !checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.push_back(contactEntity);
                dynamic_cast<Alpaca *> (contactEntity)->farmerTouch = true;
                sf_ShapeEntity->setOutlineColor(sf::Color::Green);
            }
            break;
        }
        case ID::SHOTGUN: {
            if (otherCollision == CollisionID::HIT && !checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.push_back(contactEntity);
                dynamic_cast<Shotgun *> (contactEntity)->farmerTouch = true;
            }
            break;
        }
        case ID::TRAP: {
            if (otherCollision == CollisionID::HIT && !checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.push_back(contactEntity);
                dynamic_cast<Trap *> (contactEntity)->farmerTouch = true;
            }
            break;
        }
        default:
            break;
    }
}

void Farmer::endContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            sf_DebugHit->setOutlineColor(sf::Color(100, 100, 100));
            break;
        }
        default:
            break;
    }
}

void Farmer::endContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::ALPACA: {
            if (checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.remove(contactEntity);
                dynamic_cast<Alpaca *> (contactEntity)->farmerTouch = false;
            }
            break;
        }
        case ID::SHOTGUN: {
            if (checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.remove(contactEntity);
                dynamic_cast<Shotgun *> (contactEntity)->farmerTouch = false;
            }
            break;
        }
        case ID::TRAP: {
            if (checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.remove(contactEntity);
                dynamic_cast<Trap *> (contactEntity)->farmerTouch = false;
            }
            break;
        }
        default:
            break;
    }
}

Farmer::~Farmer() {
    printf("Farmer is dead\n");
}

void Farmer::switchCurrentTexture() {

    switch (currentHealth) {
        case Health::ALIVE: {
            if (holdingEntity != nullptr) {
                if (holdingEntity->getEntity_ID() == ID::SHOTGUN) {
                    if (currentStatus == Status::AIRBORNE) {
                        sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::WALKING][spriteSwitch ? 4 : 5]);
                    } else if (currentStatus == Status::GROUNDED) {
                        sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::IDLE][2]);
                    }
                } else {
                    if (currentStatus == Status::AIRBORNE) {
                        sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::WALKING][spriteSwitch ? 2 : 3]);
                    } else if (currentStatus == Status::GROUNDED) {
                        sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::IDLE][1]);
                    }
                }
            } else {
                if (currentStatus == Status::AIRBORNE) {
                    sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::WALKING][spriteSwitch ? 0 : 1]);
                } else if (currentStatus == Status::GROUNDED) {
                    sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::IDLE][0]);
                }
            }
            break;

        }
        case Health::DEAD: {
            sf_ShapeEntity->setTexture(farmerSpriteMapPtr[Action::IDLE][3]);
            break;
        }
        case Health::GHOST: {
            if(deathClock.getElapsedTime().asSeconds() >= decayTick){
                sf_ShapeEntity->setFillColor(sf_ShapeEntity->getFillColor() - sf::Color(0, 0, 0, 1));
            }
            break;
        }
    }
}

void Farmer::renderDebugMode() {

    if (configGame->showDebugMode) {

        // Draw sf_ShapeEntity
        sf_ShapeEntity->setOutlineThickness(2);
        sf_ShapeEntity->setOutlineColor(sf::Color::Black);

        // Draw label_ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID.setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID.setRotation(sf_ShapeEntity->getRotation());
        configGame->window->draw(label_ID);

        // Draw hitSensor debug
        sf_DebugHit->setOutlineThickness(2);
        sf_DebugHit->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugHit->setRotation(body->GetAngle() * DEGtoRAD);
        configGame->window->draw(*sf_DebugHit);

    } else {
        sf_ShapeEntity->setOutlineThickness(0);
    }

}

void Farmer::pause() {
    graspClock.pause();
    movementTriggerClock.pause();
    deathClock.pause();
}

void Farmer::resume() {
    graspClock.resume();
    movementTriggerClock.resume();

    if(currentHealth != Health::ALIVE){
        deathClock.resume();
    }
}
