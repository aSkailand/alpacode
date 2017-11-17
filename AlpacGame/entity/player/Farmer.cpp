#include "Farmer.h"
#include "../alpaca/alpaca.h"

Farmer::Farmer(ConfigGame *configGame, float radius, float width, float height, float x, float y) {

    this->configGame = configGame;
    farmerMapPtr = configGame->farmerSprites;

    convertAngleToVectors((int) Action::WALKING, walkAngle);
    convertAngleToVectors((int) Action::JUMP, jumpAngle);
    convertAngleToVectors((int) Grasp::THROWING, throwAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = configGame->world->CreateBody(&bodyDef);

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
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);

    sf_HitSensor = new sf::CircleShape(radius);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(radius, radius);

    createLabel(label_ID, &this->configGame->fontID, "P1");

}


void Farmer::render(sf::RenderWindow *window) {

    // Draw sfShape Debug
    float delta_Y = sfShape->getLocalBounds().height / 2 - bodyFixture->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    // Switch Texture
    if (holdingEntity == nullptr || dynamic_cast<Shotgun*>(holdingEntity)) {
        if (currentStatus == Status::AIRBORNE) {
            sfShape->setTexture(farmerMapPtr[Action::WALKING].sprites.at(0 + static_cast<unsigned int>(spriteSwitch)));
        } else if (currentStatus == Status::GROUNDED) {
            sfShape->setTexture(farmerMapPtr[Action::IDLE].sprites.at(0));
        }
    } else{
        if (currentStatus == Status::AIRBORNE) {
            sfShape->setTexture(farmerMapPtr[Action::WALKING].sprites.at(2 + static_cast<unsigned int>(spriteSwitch)));
        } else if (currentStatus == Status::GROUNDED) {
            sfShape->setTexture(farmerMapPtr[Action::IDLE].sprites.at(1));
        }
    }

    window->draw(*sfShape);

    if (configGame->showLabels) {

        // Draw sfShape
        sfShape->setOutlineThickness(2);
        sfShape->setOutlineColor(sf::Color::Black);

        // Draw label_ID
        float offset = bodyFixture->GetShape()->m_radius + 1.f;
        label_ID->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(sfShape->getRotation());
        window->draw(*label_ID);

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

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
            currentAction = Action::WALKING;
            currentDirection = Direction::RIGHT;
            break;
        }
        case sf::Keyboard::Left: {
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

    // Flip accordingly to mouse placement
    sfShape->setScale(configGame->mouseInLeftSide ? -1.f : 1.f, 1.f);

}

void Farmer::performAction() {

    if (currentAction != Action::IDLE && currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {

        switch (currentAction) {

            case Action::WALKING: {
                forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                break;
            }
            case Action::JUMP: {
                if(configGame->mouseInLeftSide){
                    forcePushBody((int) Action::JUMP, getBody(), jumpForce, Direction::LEFT);
                }else{
                    forcePushBody((int) Action::JUMP, getBody(), jumpForce, Direction::RIGHT);
                }
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

                switch(holdingEntity->getID()){
                    case ID::ALPACA:{
                        dynamic_cast<Alpaca*>(holdingEntity)->isHeld = true;
                    }
                    case ID::SHOTGUN:{
                        dynamic_cast<Shotgun*>(holdingEntity)->isHeld = true;
                        break;
                    }
                    default:
                        break;
                }

                auto *warm = dynamic_cast<EntityWarm *> (holdingEntity);
                if (warm) {
                    warm->currentStatus = Status::AIRBORNE;
                }

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

                        b2Vec2 center = b2Vec2(sfShape->getPosition().x / SCALE, sfShape->getPosition().y / SCALE);
                        b2Vec2 offset = getBody()->GetWorldVector(b2Vec2(0.f, -2.5f));
                        holdingEntity->getBody()->SetTransform(center + offset, getBody()->GetAngle());

                        break;
                    }
                    case ID::WOLF:
                        break;
                    case ID::SHOTGUN: {

                        b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() -
                                          b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
                        float angle = atan2(-toTarget.x, toTarget.y) - 90 / DEGtoRAD;

                        b2Vec2 center = b2Vec2(sfShape->getPosition().x / SCALE, sfShape->getPosition().y / SCALE);
                        b2Vec2 offset = getBody()->GetWorldVector(b2Vec2(0.f, 0.7f));
                        holdingEntity->getBody()->SetTransform(center + offset, angle);

                        break;
                    }
                    case ID::BULLET:
                        break;
                    case ID::VOID:
                        break;
                }

            }
            break;
        }

        case Grasp::THROWING: {

            holdingEntity->physicsSensitive = true;

            switch(holdingEntity->getID()){
                case ID::ALPACA:{
                    dynamic_cast<Alpaca*>(holdingEntity)->isHeld = false;
                }
                case ID::SHOTGUN:{
                    dynamic_cast<Shotgun*>(holdingEntity)->isHeld = false;
                    break;
                }
                default:
                    break;
            }

            // Reset Rotation (So that throwing angle works as intended)
            holdingEntity->getBody()->SetTransform(holdingEntity->getBody()->GetWorldCenter(), getBody()->GetAngle());

            // Reset Speed
            holdingEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));

            // Get mouse angle
            b2Vec2 toTarget = holdingEntity->getBody()->GetWorldCenter() -
                              b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
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
            currentStatus = Status::AIRBORNE;
            sf_HitSensor->setOutlineColor(sf::Color(100, 100, 100));
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
            }
            break;
        }
        case ID::VOID:
            break;
        case ID::BULLET:
            break;
    }
}

void Farmer::startContact(Entity *contactEntity) {

    ID contactID = contactEntity->getID();

    switch (contactID) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            spriteSwitch = !spriteSwitch;
            sf_HitSensor->setOutlineColor(sf::Color::White);
            break;
        }
        case ID::FARMER:
            break;
        case ID::ALPACA: {

            if (std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(), contactEntity) ==
                currentlyTouchingEntities.end()) {
                currentlyTouchingEntities.push_back(contactEntity);
                // todo: Add farmerTouch to entity?
                auto contactAlpaca = dynamic_cast<Alpaca *> (contactEntity);
                contactAlpaca->farmerTouch = true;
                sfShape->setOutlineColor(sf::Color::Green);
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
            }
            break;
        }
        case ID::VOID:
            break;
        case ID::BULLET:
            break;
    }
}

bool Farmer::deadCheck() {
    return false;
}
