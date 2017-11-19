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
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;
    fixtureDef.shape = &b2Shape;

    /// BODY  Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / SCALE;
    b2FixtureDef bodySensor;
    bodySensor.shape = &b2Shape2;
    bodySensor.isSensor = true;
    bodySensor.filter.categoryBits = (uint16) ID::ALPACA;
    bodySensor.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::WOLF;


    // TODO: Add Detect Sensor
    // DETECTION Sensor
    b2CircleShape b2Shape3;
    b2FixtureDef detectSensor;
    b2Shape3.m_radius = (radius + 300) / SCALE;
    detectSensor.shape = &b2Shape3;
    detectSensor.isSensor = true;
    detectSensor.filter.categoryBits = (uint16) ID::ALPACA;
    detectSensor.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::WOLF;

    // Store information
    setID(Entity::ID::ALPACA);
    // TODO: Add different FixtureDef for the detect sensor
    /// SetUserData (this) implies for the whole class, which merges the two fixtureDef together

    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    bodySensorFixture = body->CreateFixture(&bodySensor);
    detectSensorFixture = body->CreateFixture(&detectSensor);

    /// Set Hit and Detect Fixture as Sensors
    bodyFixture->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    // Body Hit Sensor
    bodySensorFixture->SetUserData(convertToVoidPtr((int) CollisionID::HIT));
    // Detect Sensor
    detectSensorFixture->SetUserData(convertToVoidPtr((int) CollisionID::DETECTION));



    // Creating SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);

    sf_HitSensor = new sf::CircleShape(radius);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(radius, radius);


    // Set HP
    HP = 10;


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

    /// Normal Behavior, Check if the randomActionClock has triggered
    if (currentBehavior == Behavior::NORMAL &&
        randomActionTriggered(randomActionTick)) {
        currentAction = (Action) randomNumberGenerator(0, 1);

        switch (currentAction) {

            case Action::IDLE: {
                break;
            }
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
            default: {
                break;
            }
        }
    }
    if (currentBehavior == Behavior::AWARE) {
        currentAction = Action::IDLE;

        // TODO: 1. Print Alert sign

        // Number tick

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
    }

    if (currentBehavior == Behavior::AFRAID) {

        currentAction = Action::WALKING;
        // TODO: 1. Print Red Alert Sign


        if (behaviorClock.getElapsedTime().asSeconds() >= afraidActionTick && !detectedWithinCircle) {

            currentBehavior = Behavior::NORMAL;
            currentAction = Action::IDLE;
            behaviorClock.reset();
        }

    }

    if (currentBehavior == Behavior::FOLLOWING) {

    }
}

void Alpaca::render(sf::RenderWindow *window) {

    // Render sfShape
    float delta_Y = sfShape->getLocalBounds().height / 2 - bodyFixture->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation(body->GetAngle() * DEGtoRAD);


    // Switch Texture
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

    window->draw(*sfShape);

    if (configGame->showLabels) {

        // Draw SFShape debug
        sfShape->setOutlineThickness(2);
        if (farmerTouch) sfShape->setOutlineColor(sf::Color::Yellow);
        else sfShape->setOutlineColor(sf::Color::Black);

        /// Detect Sensor Draw
        sf_DetectSensor->setOutlineThickness(2);
        sf_DetectSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        if (currentBehavior == Behavior::AWARE) sf_DetectSensor->setOutlineColor(sf::Color::Yellow);
        else if (currentBehavior == Behavior::AFRAID) sf_DetectSensor->setOutlineColor(sf::Color::Red);
        else if (currentBehavior == Behavior::NORMAL) sf_DetectSensor->setOutlineColor(sf::Color::Green);
        window->draw(*sf_DetectSensor);

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

        // Draw label_ID
        float offset = bodyFixture->GetShape()->m_radius + 1.f;
        label_ID->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*label_ID);

        // Draw label_HP
        label_HP->setString(std::to_string(HP));
        label_HP->setPosition(getBody()->GetWorldCenter().x * SCALE,
                              getBody()->GetWorldCenter().y * SCALE);
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

    if (currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {
        switch (currentAction) {
            case Action::WALKING: {
                if (currentStatus == Status::GROUNDED) {
                    body->SetLinearVelocity(b2Vec2(0, 0));

                    if (currentBehavior == Behavior::NORMAL) {
                        forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                    } else {
                        forcePushBody((int) Action::WALKING, getBody(), runForce, currentDirection);
                    }
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

    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;

            break;
        }
        case ID::FARMER: {
            farmerTouch = false;
            break;
        }
        case ID::ALPACA:
            break;
        case ID::WOLF: {
            if (selfCollision == CollisionID::DETECTION) {
                detectedWithinCircle = false;
            }
            break;
        }

        default:
            break;
    }

}

void Alpaca::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            break;
        }
        case ID::FARMER: {
            if (selfCollision == CollisionID::HIT) {
                std::cout << "HIT SENSE" << std::endl;

            }
            /// Detects Farmer with Detect Sensor


            farmerTouch = true;
            break;
        }
        case ID::ALPACA:
            break;
        case ID::WOLF: {
            if (selfCollision == CollisionID::DETECTION) {
                // ignores detect sensor of Wolf
                if (otherCollision == CollisionID::DETECTION) break;

                detectedWithinCircle = true;
                // Find where is it detected
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());

                std::cout << "DETECT SENSE" << std::endl;
                // Becomes Aware, turns its towards the farmer
                if (currentBehavior == Behavior::NORMAL) {
                    currentBehavior = Behavior::AWARE;
                    if (delta.x > 0) {   /// Detects from the left side
                        sfShape->setScale(1.f, 1.f);
                        currentDirection = Direction::RIGHT;
                    } else {
                        sfShape->setScale(-1.f, 1.f);
                        currentDirection = Direction::LEFT;
                    }
                }

                if (currentBehavior == Behavior::AFRAID) {
                    if (delta.x <= 0) {   /// Detects from the left side
                        sfShape->setScale(1.f, 1.f);
                        currentDirection = Direction::RIGHT;
                    } else {
                        sfShape->setScale(-1.f, 1.f);
                        currentDirection = Direction::LEFT;
                    }
                }

                // Clock Resets
                behaviorClock.reset(true);

            }

            break;
        }
        default:
            break;
    }
}

bool Alpaca::deadCheck() {
    return HP <= 0;
}

Alpaca::~Alpaca() {
    printf("Alpaca %i killed.\n", id);
}

