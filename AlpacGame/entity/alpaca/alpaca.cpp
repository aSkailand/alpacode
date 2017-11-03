#include "Alpaca.h"

Alpaca::Alpaca(b2World *world, ConfigGame *configGame, float radius, float x, float y)
        : id(nextId++), Mob(id) {

    // Assign Pointers
    this->configGame = configGame;

    // Loading Textures
    loadTextures();

    // Convert angle and store unit vectors
    convertAngleToVectors((int) Action::WALKING, walkAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / 2 / SCALE;

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;
    fixtureDef.shape = &b2Shape;

    // Create Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / 2 / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::ALPACA;
    sensor.filter.maskBits = (uint16) ID::FARMER;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    sensorFixture = body->CreateFixture(&sensor);

    // Creating SFML shape
    sfShape = new sf::CircleShape(radius / 2);
    sfShape->setOrigin(radius / 2, radius / 2);
    sfShape->setTexture(&texture);
    sfShape->setOutlineThickness(2);
    sfShape->setOutlineColor(sf::Color::Black);

    // Create ID text
    createLabel(std::to_string(id), &this->configGame->fontID);

}

int Alpaca::nextId = 0;

void Alpaca::loadTextures() {
    if (!texture.loadFromFile("entity/alpaca/alpaca.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}


/// ALPACA MOVE
void Alpaca::switchAction() {

    // Check if the randomActionClock has triggered
    if (randomActionTriggered(randomActionTick)) {

        currentAction = (Action) randomNumberGenerator(0, 1);

        switch (currentAction) {
            case Action::IDLE: {
//                std::cout << "Alpaca " << id << " is now IDLE." << std::endl;
                break;
            }
            case Action::WALKING: {

                currentDirection = (Direction) randomNumberGenerator(0, 1);

                switch (currentDirection) {
                    case Direction::LEFT: {
//                        std::cout << "Alpaca " << id << " is WALKING LEFT" << std::endl;
                        sfShape->setScale(-1.f, 1.f);
                        break;
                    }
                    case Direction::RIGHT: {
//                        std::cout << "Alpaca " << id << " is WALKING RIGHT" << std::endl;
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

    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;

    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);

    if (farmerTouch) {
        sfShape->setOutlineColor(sf::Color::Green);
    } else {
        sfShape->setOutlineColor(sf::Color::Black);
    }

    if(configGame->showLabels){
        float offset = bodyFixture->GetShape()->m_radius + 1.f;
        label->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE, body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label->setRotation(sfShape->getRotation());
        window->draw(*label);
    }
}

void Alpaca::performAction() {

    if (isMovementAvailable(moveAvailableTick)) {
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
        case ID::PLANET:
            break;
        case ID::FARMER: {
            farmerTouch = false;
            break;
        }
        case ID::ALPACA:
            break;
        case ID::WOLF:
            break;
    }

}

void Alpaca::startContact(Entity *contactEntity) {

}

