#include "Farmer.h"

Farmer::Farmer(b2World *world, ConfigGame *configGame, float width, float height, float x, float y) {

    this->configGame = configGame;

    this->x = x;
    this->y = y;

    loadTextures();

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->x / SCALE, this->y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = (uint16) ID::FARMER;
    fixtureDef.filter.maskBits = (uint16) ID::PLANET;

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
//    sfShape->setTexture(&texture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);

    /// CIRCLE VERSION (SFML)
    sfShape = new sf::CircleShape(width / 2);
    sfShape->setOrigin(width / 2, width / 2);
    sfShape->setTexture(&farmerTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);
}

void Farmer::loadTextures() {
    if (!farmerTexture.loadFromFile("entity/player/farmer.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}

void Farmer::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);
}

void Farmer::switchAction() {

    // Update farmer's direction and action
    if (configGame->currentInput == sf::Keyboard::Unknown) {
        currentAction = Action::IDLE;
    } else if (configGame->currentInput == sf::Keyboard::Right) {
        sfShape->setScale(1.f, 1.f);
        currentAction = Action::WALKING;
        currentDirection = Direction::RIGHT;

    } else if (configGame->currentInput == sf::Keyboard::Left) {
        sfShape->setScale(-1.f, 1.f);
        currentAction = Action::WALKING;
        currentDirection = Direction::LEFT;
    } else if (configGame->currentInput == sf::Keyboard::Up) {
        currentAction = Action::JUMP;
    }




}

void Farmer::performAction() {
    if (currentAction != Action::IDLE) {
        if (isMovementAvailable(moveAvailableTick)) {

            switch (currentAction) {
                case Action::WALKING: {

                    float force = 5.f;
                    float mass = getBody()->GetMass();

                    switch (currentDirection) {
                        case Direction::RIGHT: {
                            b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(10.f, -10.f));
                            angle.Normalize();
                            getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                            break;
                        }
                        case Direction::LEFT: {
                            b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(-10.f, -10.f));
                            angle.Normalize();
                            getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                            break;
                        }
                    }
                    break;
                }
                case Action::JUMP: {

                    float force = 10.f;
                    float mass = getBody()->GetMass();

                    switch (currentDirection) {
                        case Direction::RIGHT: {
                            b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(15.f, -20.f));
                            angle.Normalize();
                            getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                            break;
                        }
                        case Direction::LEFT: {
                            b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(-15.f, -20.f));
                            angle.Normalize();
                            getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                            break;
                        }
                    }
                    break;
                }
                case Action::IDLE: {
                    break;
                }
            }


        }
    }
}


