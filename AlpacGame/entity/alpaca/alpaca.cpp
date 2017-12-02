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
    setID(Entity::ID::ALPACA);
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
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);

    ghostShape = new sf::RectangleShape(sf::Vector2f(width, height));
    ghostShape->setOrigin(width / 2, height / 2);
    ghostShape->setTexture(alpacaMapPtr[Action::IDLE][0]);
    ghostShape->setFillColor(sf::Color(200, 200, 200, 100));

    sf_HitSensor = new sf::CircleShape(radius);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(radius, radius);


    // Set HP
    HP = 1;


    // Create ID text
    createLabel(label_ID, &this->configGame->fontID, std::to_string(id));
    createLabel(label_HP, &this->configGame->fontID, std::to_string(HP));


    /// Initialize behavior
    currentBehavior = Behavior::NORMAL;

    /// Paint sensor
    sf_DetectSensor = new sf::CircleShape(radius + 300);
    sf_DetectSensor->setFillColor(sf::Color::Transparent);
    sf_DetectSensor->setOrigin(radius + 300, radius + 300);


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
                                sfShape->setScale(-1.f, 1.f);
                                break;
                            }
                            case Direction::RIGHT: {
                                sfShape->setScale(1.f, 1.f);
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
                    sfShape->setScale(1.f, 1.f);
                } else {
                    currentDirection = Direction::LEFT;
                    sfShape->setScale(-1.f, 1.f);
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

    /// Render sfShape

    // Switch Texture
    switch (currentHealth) {
        case Health::ALIVE: {

            switch (currentStatus) {
                case Status::GROUNDED: {
                    sfShape->setTexture(alpacaMapPtr[Action::IDLE][0]);
                    break;
                }
                case Status::AIRBORNE: {
                    if (isHeld) sfShape->setTexture(alpacaMapPtr[Action::IDLE][3]);
                    else sfShape->setTexture(alpacaMapPtr[Action::WALKING][1]);
                    break;
                }
            }
            break;

        }
        case Health::DEAD: {
            sfShape->setTexture(alpacaMapPtr[Action::IDLE][4]);
            break;
        }
        case Health::GHOST: {
            if(deathClock.getElapsedTime().asSeconds() >= decayTick){
                sfShape->setFillColor(sfShape->getFillColor() - sf::Color(0, 0, 0, 1));
            }
            break;
        }
    }


    float delta_Y = sfShape->getLocalBounds().height / 2 - fixture_body->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation(body->GetAngle() * DEGtoRAD);

    if (currentHealth == Health::GHOST) {
        b2Vec2 ghostMovementVector = getBody()->GetWorldVector(b2Vec2(0.f, -0.01f));
        ghostShape->move(sf::Vector2f(ghostMovementVector.x * SCALE, ghostMovementVector.y * SCALE));
        window->draw(*ghostShape);
    }

    window->draw(*sfShape);

    if (configGame->showLabels) {

        // Draw SFShape debug
        sfShape->setOutlineThickness(2);
        if (farmerTouch)
            sfShape->setOutlineColor(sf::Color::Yellow);
        else
            sfShape->setOutlineColor(sf::Color::Black);

        // Detect Sensor Draw
        sf_DetectSensor->setOutlineThickness(2);
        sf_DetectSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        if (currentBehavior == Behavior::AWARE)
            sf_DetectSensor->setOutlineColor(sf::Color::Yellow);
        else if (currentBehavior == Behavior::AFRAID)
            sf_DetectSensor->setOutlineColor(sf::Color::Red);
        else if (currentBehavior == Behavior::NORMAL)
            sf_DetectSensor->setOutlineColor(sf::Color::Green);
        window->draw(*sf_DetectSensor);

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

        // Draw label_ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*label_ID);

        // Draw label_HP
        label_HP->setString(std::to_string(HP));

        label_HP->setPosition(getBody()->GetWorldCenter().x * SCALE, getBody()->GetWorldCenter().y * SCALE);
        label_HP->setRotation(sfShape->getRotation());
        window->draw(*label_HP);

        switch (currentStatus) {
            case Status::GROUNDED: {
                sf_HitSensor->setOutlineColor(sf::Color::White);
                break;
            }
            case Status::AIRBORNE: {
                sf_HitSensor->setOutlineColor(sf::Color(100, 100, 100));
                break;
            }
        }

    } else {
        sfShape->setOutlineThickness(0);
        sf_HitSensor->setOutlineThickness(0);
    }
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

    switch (contactEntity->getID()) {
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
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            break;
        }
        default:
            break;
    }
}

void Alpaca::startContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
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
    switch (contactEntity->getID()) {
        case ID::WOLF: {
            if (otherCollision == CollisionID::HIT) {

                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());

                if (currentBehavior == Behavior::NORMAL) {
                    currentBehavior = Behavior::AWARE;
                    if (delta.x > 0) {
                        sfShape->setScale(1.f, 1.f);
                        currentDirection = Direction::RIGHT;
                    } else {
                        sfShape->setScale(-1.f, 1.f);
                        currentDirection = Direction::LEFT;
                    }
                }

                if (currentBehavior == Behavior::AFRAID) {
                    if (delta.x > 0) {
                        sfShape->setScale(-1.f, 1.f);
                        currentDirection = Direction::LEFT;
                    } else {
                        sfShape->setScale(1.f, 1.f);
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
    switch (contactEntity->getID()) {
        case ID::FARMER: {
            farmerTouch = false;
            break;
        }
        default:
            break;
    }
}

void Alpaca::endContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            break;
        }
        default:
            break;
    }
}

void Alpaca::endContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        default:
            break;
    }
}



