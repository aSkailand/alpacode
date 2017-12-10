#include "Farmer.h"
#include "../alpaca/alpaca.h"

Farmer::Farmer(ConfigGame *configGame, float radius, float width, float height, float x, float y) {

    this->configGame = configGame;
    farmerWithHandsMapPtr = configGame->farmerSpritesWithHands;
    farmerWithoutHandsMapPtr = configGame->farmerSpritesWithoutHands;

    convertAngleToVectors((int) Action::WALKING, walkAngle);
    convertAngleToVectors((int) Action::JUMP, jumpAngle);
    convertAngleToVectors((int) Grasp::THROWING, throwAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = configGame->world->CreateBody(&bodyDef);

    // Creating Shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / SCALE;

    // Create Fixture
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
    fixtureDef_hit.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::WOLF | (uint16) ID::SHOTGUN;

    // Store information
    setID(Entity::ID::FARMER);
    body->SetUserData((void *) this);

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef_body);
    fixture_hit = body->CreateFixture(&fixtureDef_hit);

    // Set Fixture Sense to given Enums
    fixture_body->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    fixture_hit->SetUserData(convertToVoidPtr((int) CollisionID::HIT));

    // Create SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);

    sf_HitSensor = new sf::CircleShape(radius);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(radius, radius);

    // Set HP
    HP = 10;

    // Create HitPoint barometer
    hitPointBarometer = new HitPointBarometer(this->configGame, HP, 25.f, 25.f);

    // Create ID
    label_ID = configGame->createLabel(&configGame->fontID, 20, "Farmer");


}


void Farmer::render(sf::RenderWindow *window) {

    // Draw sfShape Debug
    float delta_Y = sfShape->getLocalBounds().height / 2 - fixture_body->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    // Switch Texture
    bool isHolding = holdingEntity != nullptr;

    if (isHolding) {
        if (holdingEntity->getID() == ID::SHOTGUN) {
            if (currentStatus == Status::AIRBORNE) {
                sfShape->setTexture(farmerWithoutHandsMapPtr[Action::WALKING][spriteSwitch ? 0 : 1]);
            } else if (currentStatus == Status::GROUNDED) {
                sfShape->setTexture(farmerWithoutHandsMapPtr[Action::IDLE][0]);
            }
        } else {
            if (currentStatus == Status::AIRBORNE) {
                sfShape->setTexture(farmerWithHandsMapPtr[Action::WALKING][spriteSwitch ? 2 : 3]);
            } else if (currentStatus == Status::GROUNDED) {
                sfShape->setTexture(farmerWithHandsMapPtr[Action::IDLE][1]);
            }
        }
    } else {
        if (currentStatus == Status::AIRBORNE) {
            sfShape->setTexture(farmerWithHandsMapPtr[Action::WALKING][spriteSwitch ? 0 : 1]);
        } else if (currentStatus == Status::GROUNDED) {
            sfShape->setTexture(farmerWithHandsMapPtr[Action::IDLE][0]);
        }
    }

    window->draw(*sfShape);

    if (configGame->showLabels) {

        // Draw sfShape
        sfShape->setOutlineThickness(2);
        sfShape->setOutlineColor(sf::Color::Black);

        // Draw label_ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID.setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID.setRotation(sfShape->getRotation());
        window->draw(label_ID);

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

    } else {
        sfShape->setOutlineThickness(0);
    }
    // Draw HitPoint barometer
//    if(currentlyMousedOver) {
        hitPointBarometer->setPlacement(getBody()->GetWorldPoint(b2Vec2(0.f, -3.f)).x * SCALE,
                                        getBody()->GetWorldPoint(b2Vec2(0.f, -3.f)).y * SCALE,
                                        sfShape->getRotation());
        hitPointBarometer->render(window);
//    }
}

void Farmer::switchAction() {

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
                        currentGrasp = Grasp::HOLDING;
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
    sfShape->setScale(configGame->mouseInLeftSide ? -1.f : 1.f, 1.f);

}

void Farmer::performAction() {

    if (currentAction != Action::IDLE && currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {

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


// Check the current status of grasp
    switch (currentGrasp) {
        case Grasp::HOLDING: {

            // If farmer is in holding-mode, but holds nothing = Give him an entity to hold
            if (holdingEntity == nullptr) {
                holdingEntity = currentlyTouchingEntities.front();

                dynamic_cast<Holdable *>(holdingEntity)->isHeld = true;

                auto *warm = dynamic_cast<EntityWarm *> (holdingEntity);
                if (warm) {
                    warm->currentStatus = Status::AIRBORNE;
                }

                graspClock.restart();
            }
                // If farmer is in holding-mode, and holds something => keep holding
            else {

                switch (holdingEntity->getID()) {
                    case ID::PLANET:
                        break;
                    case ID::FARMER:
                        break;
                    case ID::ALPACA: {
                        b2Vec2 center = b2Vec2(sfShape->getPosition().x / SCALE, sfShape->getPosition().y / SCALE);
                        b2Vec2 offset = getBody()->GetWorldVector(b2Vec2(0.f, -2.5f));
                        holdingEntity->getBody()->SetTransform(center + offset, getBody()->GetAngle());
                        break;
                    }
                    case ID::WOLF:
                        break;
                    case ID::SHOTGUN: {

                        b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() -
                                          b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
                        float angle = atan2(-toTarget.x, toTarget.y) - 90 / DEGtoRAD;

                        b2Vec2 center = b2Vec2(sfShape->getPosition().x / SCALE, sfShape->getPosition().y / SCALE);
                        b2Vec2 offset = getBody()->GetWorldVector(b2Vec2(0.f, 0.7f));
                        holdingEntity->getBody()->SetTransform(center + offset, angle);

                        break;
                    }
                    case ID::BULLET:
                        break;
                    case ID::VOID:
                        break;
                }

            }
            break;
        }

        case Grasp::THROWING: {

            switch (holdingEntity->getID()) {
                case ID::ALPACA: {
                    dynamic_cast<Alpaca *>(holdingEntity)->isHeld = false;
                    break;
                }
                case ID::SHOTGUN: {
                    dynamic_cast<Shotgun *>(holdingEntity)->isHeld = false;
                    break;
                }
                default:
                    break;
            }

            // Reset Rotation (So that throwing angle works as intended)
            holdingEntity->getBody()->SetTransform(holdingEntity->getBody()->GetWorldCenter(), getBody()->GetAngle());

            // Reset Speed
            holdingEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));

            // Get mouse angle
            b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() -
                              b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
            toTarget.Normalize();

            float mass = holdingEntity->getBody()->GetMass();

            // Push body
            holdingEntity->getBody()->ApplyLinearImpulseToCenter(throwForce * mass * -toTarget, true);

//             Reset touching in case it is dropped and is still in range
//            currentlyTouchingEntities.push_back(holdingEntity);

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
    return false;
}

bool Farmer::checkIfTouching(Entity *entity) {
    return std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(),
                     entity) != currentlyTouchingEntities.end();
}

void Farmer::startContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            spriteSwitch = !spriteSwitch;
            sf_HitSensor->setOutlineColor(sf::Color::White);
            break;
        }
        default:
            break;
    }
}

void Farmer::startContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::ALPACA: {
            if(otherCollision == CollisionID::HIT && !checkIfTouching(contactEntity)) {
                currentlyTouchingEntities.push_back(contactEntity);
                dynamic_cast<Alpaca *> (contactEntity)->farmerTouch = true;
                sfShape->setOutlineColor(sf::Color::Green);
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
        default:
            break;
    }
}

void Farmer::endContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            sf_HitSensor->setOutlineColor(sf::Color(100, 100, 100));
            break;
        }
        default:
            break;
    }
}

void Farmer::endContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
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
        default:
            break;
    }
}
