#include "Wolf.h"

Wolf::Wolf(b2World *world, ConfigGame *configGame, float radius, float x, float y)
        :  id(nextId++), Mob(id){

    this->configGame = configGame;

    loadTextures();

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;

    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / 2 / SCALE;
    fixtureDef.shape = &b2Shape;

    // Store information
    setID(Entity::ID::WOLF);
    body->SetUserData((void *) this);

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    sfShape = new sf::RectangleShape(sf::Vector2f(radius, radius));
    sfShape->setOrigin(radius / 2, radius / 2);
    sfShape->setTexture(&texture);
    sfShape->setOutlineThickness(2);
    sfShape->setOutlineColor(sf::Color::Black);

    // Create ID text
    createLabel(std::to_string(id), &this->configGame->fontID);


}

int Wolf::nextId = 0;

void Wolf::switchAction() {

    // Check if it is time for randomizing the wolf's current state
    if (randomActionTriggered(randomActionTick)) {

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

    }


}


void Wolf::loadTextures() {
    if (!texture.loadFromFile("entity/wolf/wolfy.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}

void Wolf::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));
    window->draw(*sfShape);

    if(configGame->showLabels){
        label->setPosition(body->GetWorldPoint(b2Vec2(0, -3.f)).x * SCALE, body->GetWorldPoint(b2Vec2(0, -3.f)).y * SCALE);
        label->setRotation(sfShape->getRotation());
        window->draw(*label);
    }
}

void Wolf::performAction() {

    // Check if the randomActionClock has triggered
    if (isMovementAvailable(moveAvailableTick)) {

        float force = 5.f;
        float mass = getBody()->GetMass();

        if (currentAction == Action::WALKING) {
            switch (currentDirection) {
                case Direction::RIGHT: {
                    b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(10.f, -5.f));
                    angle.Normalize();
                    getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                    break;
                }
                case Direction::LEFT: {
                    b2Vec2 angle = getBody()->GetWorldVector(b2Vec2(-10.f, -5.f));
                    angle.Normalize();
                    getBody()->ApplyLinearImpulseToCenter(force * mass * angle, true);
                    break;
                }

            }
        }

    }
}

void Wolf::startContact(Entity *contactEntity) {
    std::cout << "Wolf Start Contact" << std::endl;
}

void Wolf::endContact(Entity *contactEntity) {
    std::cout << "Wolf End Contact" << std::endl;
}
