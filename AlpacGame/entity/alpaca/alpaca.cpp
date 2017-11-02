#include "Alpaca.h"

Alpaca::Alpaca(b2World *world, float width, float height, float x, float y)
        : id(nextId++), Mob(id) {

    // Loading Textures
    loadTextures();


    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = width / 2 / SCALE;

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;
    fixtureDef.shape = &b2Shape;

    b2CircleShape b2Shape2;
    b2Shape2.m_radius = width / 2 / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::ALPACA;
    sensor.filter.maskBits = (uint16) ID::FARMER;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);
    body->CreateFixture(&sensor);

    /// SQUARE VERSION (SFML)
//    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
//    sfShape->setOrigin(width / 2, height / 2);
//    sfShape->setTexture(&texture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);

    /// CIRCLE VERSION (SFML)
    sfShape = new sf::CircleShape(width / 2);
    sfShape->setOrigin(width / 2, width / 2);
    sfShape->setTexture(&texture);
    sfShape->setOutlineThickness(2);
    sfShape->setOutlineColor(sf::Color::Black);

    font.loadFromFile("entity/alpaca/pixelmix.ttf");
    label = new sf::Text();
    label->setString(std::to_string(id));
    label->setFont(font);
    label->setCharacterSize(50);
    label->setFillColor(sf::Color::Black);
//    label->setOrigin(label->getLocalBounds().width/2, label->getLocalBounds().height/2);
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
        }

    }


}

void Alpaca::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    label->setPosition(body->GetWorldPoint(b2Vec2(0,-5)).x*SCALE, body->GetWorldPoint(b2Vec2(0,-5)).y*SCALE);
    label->setRotation(sfShape->getRotation());

    window->draw(*sfShape);
    window->draw(*label);
}

void Alpaca::performAction() {

    if(farmerTouch){
        sfShape->setOutlineColor(sf::Color::Green);
    }
    else{
        sfShape->setOutlineColor(sf::Color::Black);
    }

    if (isMovementAvailable(moveAvailableTick)) {

        float force = 5.f;
        float mass = getBody()->GetMass();

        if (currentAction == Action::WALKING) {
            switch (currentDirection) {
                case Direction::RIGHT: {
                    b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(10.f, -20.f));
                    angle.Normalize();
                    getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                    break;
                }
                case Direction::LEFT: {
                    b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(-10.f, -20.f));
                    angle.Normalize();
                    getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                    break;
                }

            }
        }

    }
}

void Alpaca::endContact(Entity *contactEntity) {
//    std::cout << "Alpaca End Contact" << std::endl;
//    sfShape->setOutlineColor(sf::Color::Black);
    switch (contactEntity->getID()){
        case ID::PLANET:break;
        case ID::FARMER:{
            farmerTouch = false;
            break;
        }
        case ID::ALPACA:break;
        case ID::WOLF:break;
    }

}

void Alpaca::startContact(Entity *contactEntity) {
//    std::cout << "Alpaca Start Contact" << std::endl;
//    sfShape->setOutlineColor(sf::Color::Red);
}

