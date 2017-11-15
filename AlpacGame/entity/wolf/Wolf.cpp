#include "Wolf.h"

Wolf::Wolf(b2World *world, ConfigGame *configGame, float radius, float x, float y)
        : id(nextId++), Mob(id) {

    // Assign Pointers
    this->configGame = configGame;

    // Convert angle and store unit vectors
    convertAngleToVectors(((int) Action::WALKING), walkAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

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
    sensor.filter.categoryBits = (uint16) ID::WOLF;
    sensor.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::ALPACA;

    // Store information
    setID(Entity::ID::WOLF);
    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    bodySensorFixture = body->CreateFixture(&sensor);

    // Create SFML shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setTexture(&configGame->wolfTexture);

    // Create ID text
    createLabel(std::to_string(id), &this->configGame->fontID);


}

int Wolf::nextId = 0;

void Wolf::switchAction() {

    // Check if it is time for randomizing the wolf's current state
    if (randomActionTriggered(randomActionTick)) {

        currentAction = (Action) randomNumberGenerator(0, 1);

        switch (currentAction) {
            case Wolf::Action::WALKING: {
                currentDirection = (Direction) randomNumberGenerator(0, 1);
                switch (currentDirection) {
                    case Wolf::Direction::LEFT: {
                        sfShape->setScale(-1.f, 1.f);
                        break;
                    }
                    case Wolf::Direction::RIGHT: {
                        sfShape->setScale(1.f, 1.f);
                        break;
                    }
                }
                break;
            }
            case Action::JUMP: {
                break;
            }
            case Wolf::Action::IDLE: {
                break;
            }
        }

    }


}


void Wolf::render(sf::RenderWindow *window) {

    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;

    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);

    if (configGame->showLabels) {
        float offset = bodyFixture->GetShape()->m_radius + 1.f;
        label->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                           body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label->setRotation(sfShape->getRotation());
        window->draw(*label);
        sfShape->setOutlineColor(sf::Color::Black);
        sfShape->setOutlineThickness(2);
    } else {
        sfShape->setOutlineThickness(0);
    }
}

void Wolf::performAction() {

    // Check if the randomActionClock has triggered
    if (currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {
        switch (currentAction) {
            case Action::WALKING: {
                forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                break;
            }
            default: {
                break;
            }
        }
    }
}

void Wolf::startContact(CollisionID typeFixture, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET:
            currentStatus = Status ::GROUNDED;
            break;
        case ID::FARMER: {
            b2Vec2 delta =  getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
            float mass = contactEntity->getBody()->GetMass();
            float tempAngle = attackAngle;

            if(delta.x > 0){ /// Hit from the right side, force must be going left
                tempAngle = 180 - tempAngle; }

            b2Vec2 dir = contactEntity->getBody()->GetWorldVector(b2Vec2( -cos(tempAngle *b2_pi/180), -sin( tempAngle* b2_pi/180)));

            contactEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
            contactEntity->getBody()->ApplyLinearImpulseToCenter(mass* attackForce * dir, true);
            break;

        }
        case ID::ALPACA: {

            b2Vec2 delta =  getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
            float mass = contactEntity->getBody()->GetMass();
            float tempAngle = attackAngle;

            if(delta.x > 0){ /// Hit from the right side, force must be going left
                tempAngle = 180 - tempAngle; }

            b2Vec2 dir = contactEntity->getBody()->GetWorldVector(b2Vec2( -cos(tempAngle *b2_pi/180), -sin( tempAngle* b2_pi/180)));

            contactEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
            contactEntity->getBody()->ApplyLinearImpulseToCenter(mass* attackForce * dir, true);
            break;
        }

        case ID::WOLF:
            break;
    }
}

void Wolf::endContact(CollisionID typeCollision, Entity *contactEntity) {
    switch (contactEntity->getID()){
        case ID::PLANET:{
            currentStatus = Status::AIRBORNE;
            break;
        }
        case ID::ALPACA:{
            break;
        }
        case ID::FARMER:{
            break;
        }

    }



}
