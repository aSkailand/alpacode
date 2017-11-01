#include "Wolf.h"

Wolf::Wolf(b2World *world, b2Body *planetBody, float width, float height, float x, float y) : id(nextId++) {

    this->planetBody = planetBody;

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
    fixtureDef.filter.categoryBits = (uint16) ID::WOLF;
    fixtureDef.filter.maskBits = (uint16) ID::PLANET;

    b2CircleShape b2Shape;
    b2Shape.m_radius = width / 2 / SCALE;
    fixtureDef.shape = &b2Shape;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);
    sfShape->setTexture(&wolfTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);

    // Creating Random Number Generator
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count() + id;
    generator = std::default_random_engine(seed);

    moveTimer.restart();

}

int Wolf::nextId = 0;

void Wolf::switchAction() {

    // Check if it is time for randomizing the wolf's current state
    if ((int) clock.getElapsedTime().asSeconds() >= tickSecond) {

        currentAction = (Action) randomNumberGenerator(0, 1);

        switch (currentAction) {
            case Wolf::Action::IDLE: {
                std::cout << "Wolf " << id << " is now IDLE." << std::endl;
                break;
            }
            case Wolf::Action::WALKING: {
                currentDirection = (Direction) randomNumberGenerator(0, 1);
                switch (currentDirection) {
                    case Wolf::Direction::LEFT: {
                        std::cout << "Wolf " << id << " is WALKING LEFT" << std::endl;
                        sfShape->setScale(-1.f, 1.f);
                        break;
                    }
                    case Wolf::Direction::RIGHT: {
                        std::cout << "Wolf " << id << " is WALKING RIGHT" << std::endl;
                        sfShape->setScale(1.f, 1.f);
                        break;
                    }
                }
                break;
            }
        }

        clock.restart();

    }

    // Check if the clock has triggered
    if (moveTimer.getElapsedTime().asSeconds() >= moveCoolDown) {

        if (currentAction == Action::WALKING) {
            switch (currentDirection) {
                case Direction::LEFT: {
                    getBody()->ApplyLinearImpulseToCenter(10.f * getBody()->GetWorldVector(b2Vec2(-10.f, -10.f)),
                                                          true);
                    break;
                }
                case Direction::RIGHT: {

                    getBody()->ApplyLinearImpulseToCenter(10.f * getBody()->GetWorldVector(b2Vec2(10.f, -10.f)),
                                                          true);
                    break;
                }
            }
        }

        moveTimer.restart();
    }
}

int Wolf::randomNumberGenerator(int lower, int upper) {
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}

void Wolf::loadTextures() {
    if (!wolfTexture.loadFromFile("entity/wolf/wolfy.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}

void Wolf::adjust() {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));
}
