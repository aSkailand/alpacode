#include "alpaca.h"

Alpaca::Alpaca(ConfigGame *configGame, float radius, float width, float height, float x, float y)
        : id(nextId++), Mob(id) {

    // Assign Pointers
    this->configGame = configGame;
    alpacaMapPtr = configGame->alpacaSprites;

    // Convert angle and store unit vectors
    convertAngleToVectors((int) Action::WALKING, walkAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = configGame->world->CreateBody(&bodyDef);

    // Create Fixture
    b2CircleShape b2Shape_1;
    b2Shape_1.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_body;
    fixtureDef_body.density = density;
    fixtureDef_body.friction = friction;
    fixtureDef_body.restitution = restitution;
    fixtureDef_body.filter.categoryBits = categoryBits;
    fixtureDef_body.filter.maskBits = maskBits;
    fixtureDef_body.shape = &b2Shape_1;

    // Body sensor
    b2CircleShape b2Shape_2;
    b2Shape_2.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_hit;
    fixtureDef_hit.shape = &b2Shape_2;
    fixtureDef_hit.isSensor = true;
    fixtureDef_hit.filter.categoryBits = (uint16) ID::ALPACA;
    fixtureDef_hit.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::WOLF;

    // Detection sensor
    b2CircleShape b2Shape_3;
    b2FixtureDef fixtureDef_detection;
    b2Shape_3.m_radius = (radius + 300) / SCALE;
    fixtureDef_detection.shape = &b2Shape_3;
    fixtureDef_detection.isSensor = true;
    fixtureDef_detection.filter.categoryBits = (uint16) ID::ALPACA;
    fixtureDef_detection.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::WOLF;

    // Store information
    setEntity_ID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef_body);
    fixture_hit = body->CreateFixture(&fixtureDef_hit);
    fixture_detection = body->CreateFixture(&fixtureDef_detection);

    // Set Hit and Detect Fixture as Sensors
    fixture_body->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    fixture_hit->SetUserData(convertToVoidPtr((int) CollisionID::HIT));
    fixture_detection->SetUserData(convertToVoidPtr((int) CollisionID::DETECTION));

    // Creating SFML shape
    sf_ShapeEntity = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_ShapeEntity->setOrigin(width / 2, height / 2);

    sf_ShapeGhost = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_ShapeGhost->setOrigin(width / 2, height / 2);
    sf_ShapeGhost->setTexture(alpacaMapPtr[Action::IDLE][0]);
    sf_ShapeGhost->setFillColor(sf::Color(200, 200, 200, 100));

    sf_DebugHit = new sf::CircleShape(radius);
    sf_DebugHit->setFillColor(sf::Color::Transparent);
    sf_DebugHit->setOrigin(radius, radius);


    // Set HP
    HP = 1;


    // Create ID text
    createLabel(label_ID, &this->configGame->fontID, std::to_string(id));
    createLabel(label_HP, &this->configGame->fontID, std::to_string(HP));


    /// Initialize behavior
    currentBehavior = Behavior::NORMAL;

    /// Paint sensor
    sf_DebugDetection = new sf::CircleShape(radius + 300);
    sf_DebugDetection->setFillColor(sf::Color::Transparent);
    sf_DebugDetection->setOrigin(radius + 300, radius + 300);

    /// Initialize clocks
    randomActionClock.reset(true);
    movementTriggerClock.reset(true);

}

int Alpaca::nextId = 0;

void Alpaca::switchAction() {

    // Check current health and update according to current health
    handleHealth();

    // Cancel if not alive
    if (currentHealth != Health::ALIVE)
        return;


    switch (currentBehavior) {
        case Behavior::NORMAL: {
            if (randomActionTriggered(randomActionTick)) {
                currentAction = (Action) randomNumberGenerator(0, 1);
                switch (currentAction) {
                    case Action::WALKING: {
                        currentDirection = (Direction) randomNumberGenerator(0, 1);
                        switch (currentDirection) {
                            case Direction::LEFT: {
                                sf_ShapeEntity->setScale(-1.f, 1.f);
                                break;
                            }
                            case Direction::RIGHT: {
                                sf_ShapeEntity->setScale(1.f, 1.f);
                                break;
                            }
                        }
                    }
                    case Action::IDLE: {
                        break;
                    }
                    case Action::JUMP: {
                        break;
                    }
                }

            }
            break;
        }
        case Behavior::AWARE: {
            currentAction = Action::IDLE;

            if (behaviorClock.getElapsedTime().asSeconds() >= awareActionTick) {
                currentBehavior = Behavior::AFRAID;

                if (currentDirection == Direction::LEFT) {
                    currentDirection = Direction::RIGHT;
                    sf_ShapeEntity->setScale(1.f, 1.f);
                } else {
                    currentDirection = Direction::LEFT;
                    sf_ShapeEntity->setScale(-1.f, 1.f);
                }

                // Reset defaults to false
                behaviorClock.reset(true);
            }
            break;
        }
        case Behavior::AFRAID: {
            currentAction = Action::WALKING;
            if (behaviorClock.getElapsedTime().asSeconds() >= afraidActionTick) {
                currentBehavior = Behavior::NORMAL;
                currentAction = Action::IDLE;
                behaviorClock.reset(false);
            }
            break;
        }
        case Behavior::FOLLOWING: {
            break;
        }
    }
}

void Alpaca::render(sf::RenderWindow *window) {


    switchCurrentTexture();

    calcShapeEntityPlacement();

    if (currentHealth == Health::GHOST) {
        renderDeath();
        window->draw(*sf_ShapeGhost);
    }

    window->draw(*sf_ShapeEntity);

    renderDebugMode();


}

void Alpaca::performAction() {

    if (currentHealth != Health::ALIVE)
        return;

    if (currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {
        switch (currentAction) {
            case Action::WALKING: {
                body->SetLinearVelocity(b2Vec2(0, 0));

                if (currentBehavior == Behavior::NORMAL) {
                    forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                } else {
                    forcePushBody((int) Action::WALKING, getBody(), runForce, currentDirection);
                }

                break;
            }
            case Action::JUMP:
                break;
            case Action::IDLE:
                break;
        }
    }
}


void Alpaca::endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (selfCollision) {
        case CollisionID::BODY: {
            endContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::HIT: {
            endContact_hit(otherCollision, contactEntity);
            break;
        }
        case CollisionID::DETECTION: {
            endContact_detection(otherCollision, contactEntity);
            break;
        }
    }

}

void Alpaca::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (selfCollision) {
        case CollisionID::BODY: {
            startContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::HIT: {
            startContact_hit(otherCollision, contactEntity);
            break;
        }
        case CollisionID::DETECTION: {
            startContact_detection(otherCollision, contactEntity);
            break;
        }
    }

    switch (contactEntity->getEntity_ID()) {
        case ID::WOLF: {
            if (selfCollision == CollisionID::DETECTION) {
                // ignores detect sensor of Wolf

            }

            break;
        }
        default:
            break;
    }
}

bool Alpaca::deadCheck() {
    return currentHealth == Health::GHOST && !deathClock.isRunning();
}

Alpaca::~Alpaca() {
    printf("Alpaca %i is dead.\n", id);
}

void Alpaca::startContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            break;
        }
        default:
            break;
    }
}

void Alpaca::startContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::FARMER: {
            farmerTouch = true;
            break;
        }
        default: {
            break;
        }
    }
}

void Alpaca::startContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::WOLF: {
            if (otherCollision == CollisionID::HIT) {

                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());

                if (currentBehavior == Behavior::NORMAL) {
                    currentBehavior = Behavior::AWARE;
                    if (delta.x > 0) {
                        sf_ShapeEntity->setScale(1.f, 1.f);
                        currentDirection = Direction::RIGHT;
                    } else {
                        sf_ShapeEntity->setScale(-1.f, 1.f);
                        currentDirection = Direction::LEFT;
                    }
                }

                if (currentBehavior == Behavior::AFRAID) {
                    if (delta.x > 0) {
                        sf_ShapeEntity->setScale(-1.f, 1.f);
                        currentDirection = Direction::LEFT;
                    } else {
                        sf_ShapeEntity->setScale(1.f, 1.f);
                        currentDirection = Direction::RIGHT;
                    }
                }

                // Clock Resets
                behaviorClock.reset(true);
            }
            break;
        }
        default: {
            break;
        }
    }


}

void Alpaca::endContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::FARMER: {
            farmerTouch = false;
            break;
        }
        default:
            break;
    }
}

void Alpaca::endContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            break;
        }
        default:
            break;
    }
}

void Alpaca::endContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        default:
            break;
    }
}

void Alpaca::performHold() {

}

void Alpaca::performThrow() {

}

void Alpaca::switchCurrentTexture() {

    switch (currentHealth) {
        case Health::ALIVE: {

            switch (currentStatus) {
                case Status::GROUNDED: {
                    sf_ShapeEntity->setTexture(alpacaMapPtr[Action::IDLE][0]);
                    break;
                }
                case Status::AIRBORNE: {
                    if (isHeld) sf_ShapeEntity->setTexture(alpacaMapPtr[Action::IDLE][3]);
                    else sf_ShapeEntity->setTexture(alpacaMapPtr[Action::WALKING][1]);
                    break;
                }
            }
            break;

        }
        case Health::DEAD: {
            sf_ShapeEntity->setTexture(alpacaMapPtr[Action::IDLE][4]);
            break;
        }
        case Health::GHOST: {
            break;
        }
    }
}

void Alpaca::renderDebugMode() {
    if (configGame->showDebugMode) {

        // Draw SFShape debug
        sf_ShapeEntity->setOutlineThickness(2);
        if (farmerTouch)
            sf_ShapeEntity->setOutlineColor(sf::Color::Yellow);
        else
            sf_ShapeEntity->setOutlineColor(sf::Color::Black);

        // Detect Sensor Draw
        sf_DebugDetection->setOutlineThickness(2);
        sf_DebugDetection->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        if (currentBehavior == Behavior::AWARE)
            sf_DebugDetection->setOutlineColor(sf::Color::Yellow);
        else if (currentBehavior == Behavior::AFRAID)
            sf_DebugDetection->setOutlineColor(sf::Color::Red);
        else if (currentBehavior == Behavior::NORMAL)
            sf_DebugDetection->setOutlineColor(sf::Color::Green);
        configGame->window->draw(*sf_DebugDetection);

        // Draw hitSensor debug
        sf_DebugHit->setOutlineThickness(2);
        sf_DebugHit->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugHit->setRotation(body->GetAngle() * DEGtoRAD);
        configGame->window->draw(*sf_DebugHit);

        // Draw label_ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(body->GetAngle() * DEGtoRAD);
        configGame->window->draw(*label_ID);

        // Draw label_HP
        label_HP->setString(std::to_string(HP));

        label_HP->setPosition(getBody()->GetWorldCenter().x * SCALE, getBody()->GetWorldCenter().y * SCALE);
        label_HP->setRotation(sf_ShapeEntity->getRotation());
        configGame->window->draw(*label_HP);

        switch (currentStatus) {
            case Status::GROUNDED: {
                sf_DebugHit->setOutlineColor(sf::Color::White);
                break;
            }
            case Status::AIRBORNE: {
                sf_DebugHit->setOutlineColor(sf::Color(100, 100, 100));
                break;
            }
        }

    } else {
        sf_ShapeEntity->setOutlineThickness(0);
        sf_DebugHit->setOutlineThickness(0);
    }
}




void Alpaca::pause() {
    movementTriggerClock.pause();
    randomActionClock.pause();
    behaviorClock.pause();
    deathClock.pause();
}

void Alpaca::resume() {
    movementTriggerClock.resume();
    randomActionClock.resume();
    behaviorClock.resume();

    if(currentHealth != Health::ALIVE){
        deathClock.resume();
    }
}

