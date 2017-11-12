#include "Farmer.h"
#include "../alpaca/alpaca.h"

Farmer::Farmer(b2World *world, ConfigGame *configGame, float radius, float x, float y) {

    this->configGame = configGame;

    convertAngleToVectors((int) Action::WALKING, walkAngle);
    convertAngleToVectors((int) Action::JUMP, jumpAngle);
    convertAngleToVectors((int) Grasp::THROWING, throwAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Creating Shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / SCALE;

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;
    fixtureDef.shape = &b2Shape;


    // Create sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::FARMER;
    sensor.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::WOLF | (uint16) ID::SHOTGUN;

    // Store information
    setID(Entity::ID::FARMER);
    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    sensorFixture = body->CreateFixture(&sensor);

    // Create SFML shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setTexture(&configGame->farmerTexture);

    // Create ID text
    createLabel("P1", &this->configGame->fontID);
}


void Farmer::render(sf::RenderWindow *window) {
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
        sfShape->setOutlineThickness(2);
    } else {
        sfShape->setOutlineThickness(0);
    }
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
                }
                else if (currentGrasp == Grasp::HOLDING) {
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

                holdingEntity->physicsSensitive = false;

                // todo: add a if else instead? Or add distinctive if cases that fit all entity?
                auto *warm = dynamic_cast<EntityWarm *> (holdingEntity);
                if (warm) {
                    warm->currentStatus = Status::AIRBORNE;
                }

//                auto *cold = dynamic_cast<EntityCold *> (holdingEntity);
//                if (cold) {
//
//                }


                graspClock.restart();
            }
                // If farmer is in holding-mode, and holds something => keep holding
            else {

                switch (holdingEntity->getID()) {
                    case ID::PLANET:
                        break;
                    case ID::FARMER:
                        break;
                    case ID::ALPACA: {
                        // Calculate the length between farmer and the held entity.
                        float offset = -1.f;
                        float32 delta =
                                bodyFixture->GetShape()->m_radius + holdingEntity->bodyFixture->GetShape()->m_radius;

                        // Lock the entity delta length above farmer's origin
                        holdingEntity->getBody()->SetTransform(getBody()->GetWorldPoint(b2Vec2(0, -(delta + offset))),
                                                              getBody()->GetAngle());
                        break;
                    }
                    case ID::WOLF:
                        break;
                    case ID::SHOTGUN: {
                        // Calculate the length between farmer and the held entity.
                        float offsetY = -2.5f;
                        float offsetX = 0.5f;

                        if(currentDirection == Direction::LEFT){
                            offsetX *= -1;
                        }

                        float32 delta =
                                bodyFixture->GetShape()->m_radius + holdingEntity->bodyFixture->GetShape()->m_radius;




//                        float bodyAngle = body->GetAngle();
//                        float posY = configGame->winPdowHeigth - configGame->mouseYpos;
                        b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() - b2Vec2( configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
                        float angle = atan2(-toTarget.x, toTarget.y) - 90 / DEGtoRAD;
//                        while ( angle < -180 * DEGtoRAD ) angle += 360 * DEGtoRAD;
//                        while ( angle >  180 * DEGtoRAD ) angle -= 360 * DEGtoRAD;
//                        printf("Angle: %f\n",angle * DEGtoRAD);
//                        float desiredAngle = atan2f( -toTarget.x, toTarget.y );
//                        while ( desiredAngle < -180 * DEGtoRAD ) desiredAngle += 360 * DEGtoRAD;
//                        while ( desiredAngle >  180 * DEGtoRAD ) desiredAngle -= 360 * DEGtoRAD;

//                        b2Vec2 angleVec(
//                                holdingEntity->getBody()->GetLocalPoint(b2Vec2(configGame->mouseXpos, configGame->mouseYpos).x,));

                        // Lock the entity delta length above farmer's origin
                        holdingEntity->getBody()->SetTransform(getBody()->GetWorldPoint(b2Vec2(offsetX, -(delta + offsetY))),
                                                               angle);

//                        holdingEntity->getBody()->SetTransform(getBody()->GetWorldCenter(),holdingEntity->getBody()->GetWorldVector(toTarget));
                        break;
                    }
                }

            }
            break;
        }

        case Grasp::THROWING: {

            holdingEntity->physicsSensitive = true;

            // Reset Rotation (So that throwing angle works as intended)
            holdingEntity->getBody()->SetTransform(holdingEntity->getBody()->GetWorldCenter(), getBody()->GetAngle());

            // Reset Speed
            holdingEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));

            // Get mouse angle
            b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() - b2Vec2( configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
            toTarget.Normalize();

            float mass = holdingEntity->getBody()->GetMass();

            holdingEntity->getBody()->ApplyLinearImpulseToCenter(throwForce * mass * -toTarget, true);

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
        case ID::SHOTGUN: {
            if (std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(), contactEntity) !=
                currentlyTouchingEntities.end()) {
                currentlyTouchingEntities.remove(contactEntity);
                auto contactAlpaca = dynamic_cast<Shotgun *> (contactEntity);
                contactAlpaca->farmerTouch = false;
                std::cout << "Touching " << currentlyTouchingEntities.size() << " entities." << std::endl;

            }
            break;
        }
    }
}

void Farmer::startContact(Entity *contactEntity) {

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
        case ID::WOLF: {
            break;
        }
        case ID::SHOTGUN: {
            if (std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(),
                          contactEntity) == currentlyTouchingEntities.end()) {
                currentlyTouchingEntities.push_back(contactEntity);
                // todo: Add farmerTouch to entity?
                auto contactAlpaca = dynamic_cast<Shotgun *> (contactEntity);
                contactAlpaca->farmerTouch = true;
                sfShape->setOutlineColor(sf::Color::Green);

                std::cout << "Touching " << currentlyTouchingEntities.size() << " entities." << std::endl;
            }
            break;
        }
    }
}

bool Farmer::deadCheck() {
    return false;
}
