#include "Farmer.h"
#include "../alpaca/alpaca.h"

Farmer::Farmer(b2World *world, ConfigGame *configGame, float width, float height, float x, float y) {

    this->configGame = configGame;

    this->x = x;
    this->y = y;

    convertAngleToVectors((int) Action::WALKING, walkAngle);
    convertAngleToVectors((int) Action::JUMP, jumpAngle);
    convertAngleToVectors((int) Grasp::THROWING, throwAngle);
//    convertAngleToVectors(jumpAngle, jumpVec);
//    convertAngleToVectors(throwAngle, throwVec);

    loadTextures();

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->x / SCALE, this->y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Creating Shape
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
    sensor.filter.categoryBits = (uint16) ID::FARMER;
    sensor.filter.maskBits = (uint16) ID::ALPACA;

    // Store information
    setID(Entity::ID::FARMER);
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
}

void Farmer::loadTextures() {
    if (!texture.loadFromFile("entity/player/farmer.png")) {
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

    switch (configGame->currentInput) {
        case sf::Keyboard::Up: {
            currentAction = Action::JUMP;
            break;
        }
        case sf::Keyboard::Right: {
            sfShape->setScale(1.f, 1.f);
            currentAction = Action::WALKING;
            currentDirection = Direction::RIGHT;
            break;
        }
        case sf::Keyboard::Left: {
            sfShape->setScale(-1.f, 1.f);
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


}

void Farmer::performAction() {

    if (currentAction != Action::IDLE && isMovementAvailable(moveAvailableTick)) {

        switch (currentAction) {
            case Action::WALKING: {
                forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                break;
            }
            case Action::JUMP: {
                forcePushBody((int) Action::JUMP, getBody(), jumpForce, currentDirection);
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
                currentlyTouchingEntities.pop_front();
                graspClock.restart();
            } else {
                // If farmer is in holding-mode, and holds something => keep holding.
                holdingEntity->getBody()->SetTransform(getBody()->GetWorldPoint(b2Vec2(0, -4)), getBody()->GetAngle());
            }
            break;
        }

        case Grasp::THROWING: {

            holdingEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
            forcePushBody((int) Grasp::THROWING, holdingEntity->getBody(), throwForce, currentDirection);
            holdingEntity = nullptr;
            currentGrasp = Grasp::EMPTY;
            break;
        }

        case Grasp::EMPTY: {
            break;
        }

    }

}

void Farmer::endContact(Entity *contactEntity) {

    ID contactID = contactEntity->getID();
    sfShape->setOutlineColor(sf::Color::Black);

    switch (contactID) {
        case ID::PLANET: {
            sfShape->setOutlineColor(sf::Color::Red);
            break;
        }
        case ID::FARMER:
            break;
        case ID::ALPACA: {

            if (std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(), contactEntity) !=
                currentlyTouchingEntities.end()) {
                currentlyTouchingEntities.remove(contactEntity);
                auto contactAlpaca = dynamic_cast<Alpaca *> (contactEntity);
                contactAlpaca->farmerTouch = false;
                std::cout << "Touching " << currentlyTouchingEntities.size() << " entities." << std::endl;

            }

            break;

        }
        case ID::WOLF:
            break;
    }
}

void Farmer::startContact(Entity *contactEntity) {
//    std::cout << "Farmer Start Contact" << std::endl;

    ID contactID = contactEntity->getID();

    switch (contactID) {
        case ID::PLANET: {
            sfShape->setOutlineColor(sf::Color::Black);
            break;
        }
        case ID::FARMER:
            break;
        case ID::ALPACA: {

            if (std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(),
                          contactEntity) == currentlyTouchingEntities.end()) {
                currentlyTouchingEntities.push_back(contactEntity);
                // todo: Add farmerTouch to entity?
                auto contactAlpaca = dynamic_cast<Alpaca *> (contactEntity);
                contactAlpaca->farmerTouch = true;
                sfShape->setOutlineColor(sf::Color::Green);

                std::cout << "Touching " << currentlyTouchingEntities.size() << " entities." << std::endl;
            }


        }
        case ID::WOLF:
            break;
    }
}
