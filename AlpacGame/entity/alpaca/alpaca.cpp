#include "alpaca.h"

Alpaca::Alpaca(ConfigGame *configGame, float radius, float width, float height, float x, float y)
        : id(nextId++), Mob(id) {

    // Assign Pointers
    this->configGame = configGame;

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

    // Create Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::ALPACA;
    sensor.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::WOLF;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    sensorFixture = body->CreateFixture(&sensor);

    // Creating SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);
    sfShape->setTexture(&configGame->alpacaTexture);

    sf_HitSensor = new sf::CircleShape(radius);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(radius, radius);


    // Set HP
    HP = 10;

    // Create ID text
    createLabel(label_ID, &this->configGame->fontID, std::to_string(id));
    createLabel(label_HP, &this->configGame->fontID, std::to_string(HP));


}

int Alpaca::nextId = 0;

void Alpaca::switchAction() {

    // Check if the randomActionClock has triggered
    if (currentStatus == Status::GROUNDED && randomActionTriggered(randomActionTick)) {

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


}

void Alpaca::render(sf::RenderWindow *window) {

    // Render sfShape
    float delta_Y = sfShape->getLocalBounds().height/2 - bodyFixture->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation(body->GetAngle() * DEGtoRAD);

    window->draw(*sfShape);


    if (configGame->showLabels) {

        // Draw SFShape debug
        sfShape->setOutlineThickness(2);
        sfShape->setOutlineColor(sf::Color::Black);

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

//        if (farmerTouch) sfShape->setOutlineColor(sf::Color::Green);
//        else sfShape->setOutlineColor(sf::Color::Black);


    } else {
        sfShape->setOutlineThickness(0);
        sf_HitSensor->setOutlineThickness(0);
    }
}

void Alpaca::performAction() {

    if (currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {
        switch (currentAction) {
            case Action::WALKING: {
                forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                break;
            }
            case Action::JUMP:
                break;
            case Action::IDLE:
                break;
        }
    }
}

void Alpaca::endContact(Entity *contactEntity) {

    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            sf_HitSensor->setOutlineColor(sf::Color(100, 100, 100));
            break;
        }
        case ID::FARMER: {
            farmerTouch = false;
            break;
        }
        case ID::ALPACA:
            break;
        case ID::WOLF:
            break;
        default:
            break;
    }

}

void Alpaca::startContact(Entity *contactEntity) {

    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            sf_HitSensor->setOutlineColor(sf::Color::White);
            break;
        }
        case ID::FARMER: {
            farmerTouch = true;
            break;
        }
        case ID::ALPACA:
            break;
        case ID::WOLF:
            break;
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

