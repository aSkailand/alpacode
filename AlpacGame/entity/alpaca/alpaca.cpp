#include "Alpaca.h"

Alpaca::Alpaca(b2World *world, b2Body* planetBody, float width, float height, float x, float y) : id(nextId++) {

    this->planetBody = planetBody;

    // Loading Textures
    loadTextures();

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 15.0f;
    fixtureDef.restitution = 0.0f;

    b2CircleShape b2Shape;
    b2Shape.m_radius = width / 2 / SCALE;
    fixtureDef.shape = &b2Shape;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    /// SQUARE VERSION (SFML)
//    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
//    sfShape->setOrigin(width / 2, height / 2);
//    sfShape->setTexture(&alpacaTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);

    /// CIRCLE VERSION (SFML)
    sfShape = new sf::CircleShape(width / 2);
    sfShape->setOrigin(width / 2, width / 2);
    sfShape->setTexture(&alpacaTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);


    // Creating Random Number Generator
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count() + id;
    generator = std::default_random_engine(seed);

    // Initiate internal clock
    clock.restart();


}

int Alpaca::nextId = 0;

int Alpaca::randomNumberGenerator(int lower, int upper) {
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}

void Alpaca::loadTextures() {
    if (!alpacaTexture.loadFromFile("entity/alpaca/alpaca.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}

void Alpaca::adjust() {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));
}

/// ALPACA MOVE
void Alpaca::switchAction() {

    // Check if the clock has triggered
    if (clock.getElapsedTime().asSeconds() >= tickSecond) {

        currentAction = (Action) randomNumberGenerator(0, 1);

        switch (currentAction) {
            case Action::IDLE: {
                std::cout << "Alpaca " << id << " is now IDLE." << std::endl;
                break;
            }
            case Action::WALKING: {

                currentDirection = (Direction) randomNumberGenerator(0, 1);

                switch (currentDirection) {
                    case Direction::LEFT: {
                        std::cout << "Alpaca " << id << " is WALKING LEFT" << std::endl;
                        sfShape->setScale(-1.f, 1.f);
                        break;
                    }
                    case Direction::RIGHT: {
                        std::cout << "Alpaca " << id << " is WALKING RIGHT" << std::endl;
                        sfShape->setScale(1.f, 1.f);
                        break;
                    }
                }
            }
        }

        clock.restart();
    }

    if (currentAction == Action::WALKING) {
        switch (currentDirection) {
            case Direction::LEFT: {


                b2Vec2 delta = planetBody->GetWorldCenter() - getBody()->GetWorldCenter();

                getBody()->ApplyLinearImpulse((getBody()->GetWorldVector(b2Vec2(-0.1f, 0)) + delta),
                                                      getBody()->GetWorldCenter(), true);
                break;
            }
            case Direction::RIGHT: {


                b2Vec2 delta = planetBody->GetWorldCenter() - getBody()->GetWorldCenter();

                getBody()->ApplyLinearImpulse((getBody()->GetWorldVector(b2Vec2(0.1f, 0)) + delta),
                                              getBody()->GetWorldCenter(), true);
                break;
            }
        }
    }


}
