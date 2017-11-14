#include "Farmer.h"
#include "../alpaca/alpaca.h"

Farmer::Farmer(b2World *world, ConfigGame *configGame, float radius, float x, float y) {

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
    sensor.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::WOLF;

    // Store information
    setID(Entity::ID::FARMER);
    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    sensorFixture = body->CreateFixture(&sensor);

    // Create SFML shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    //getting the starting sprite for farmer from spriteMap;
    sfShape->setTexture(farmerMapPtr[Action::IDLE].sprites.at(0));
    //sfShape->setTexture(&configGame->farmerTexture);

    // Create ID text
    createLabel("P1", &this->configGame->fontID);

    /// Check if player starts in the air
    currentStatus = Status::AIRBORNE;
}


void Farmer::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);

    if(configGame->showLabels){
        float offset = bodyFixture->GetShape()->m_radius + 1.f;
        label->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE, body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label->setRotation(sfShape->getRotation());
        window->draw(*label);
        sfShape->setOutlineThickness(2);
    } else{
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

    if (currentAction != Action::IDLE  && currentStatus == Status::GROUNDED && isMovementAvailable(moveAvailableTick)) {

        switch (currentAction) {

            case Action::WALKING: {
                forcePushBody((int) Action::WALKING, getBody(), walkForce, currentDirection);
                // TODO: Walking ANimation
                break;
            }
            case Action::JUMP: {
                forcePushBody((int) Action::JUMP, getBody(), jumpForce, currentDirection);
                break;
            }
            case Action::IDLE: {
                //TODO: IDle Animation
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

                dynamic_cast<EntityWarm*> (holdingEntity)->currentStatus = Status::AIRBORNE;

                graspClock.restart();
            }
                // If farmer is in holding-mode, and holds something => keep holding
            else {
                // Calculate the length between farmer and the held entity.
                float offset = -1.f;
                float32 delta = bodyFixture->GetShape()->m_radius + holdingEntity->bodyFixture->GetShape()->m_radius;

                // Lock the entity delta length above farmer's origin
                holdingEntity->getBody()->SetTransform(getBody()->GetWorldPoint(b2Vec2(0, -(delta+offset))), getBody()->GetAngle());
                // TODO: HOlding animation
                if(currentAction == Action::WALKING){

                }
                else if(currentAction == Action::IDLE){
                    sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(1));
                }
                holdingSwitch = true;
            }
            break;
        }

        case Grasp::THROWING: {

            holdingEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
            forcePushBody((int) Grasp::THROWING, holdingEntity->getBody(), throwForce, currentDirection);
            holdingEntity = nullptr;
            currentGrasp = Grasp::EMPTY;

            if(currentAction == Action::WALKING){
                sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(0));
            }
            else if(currentAction == Action::IDLE){
                sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(0));
            }
            holdingSwitch = false;
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
            currentStatus = Status::AIRBORNE;
            // TODO: Jump ANimation

            if(holdingSwitch){
                if(!spriteSwitch){
                    if(currentAction == Action::WALKING || currentAction == Action::JUMP){
                        sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(4));
                        spriteSwitch = true;
                    }
                }
                else{
                    if(currentAction == Action::WALKING || currentAction == Action::JUMP){
                        sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(5));
                        spriteSwitch = false;
                    }
                }
            }
            else{
                if(!spriteSwitch){
                    if(currentAction == Action::WALKING || currentAction == Action::JUMP){
                        sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(1));
                        spriteSwitch = true;
                    }

                }
                else{
                    if(currentAction == Action::WALKING || currentAction == Action::JUMP){
                        sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(2));
                        spriteSwitch = false;
                    }
                }
            }


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
            currentStatus = Status::GROUNDED;

            // TODO: Ground Animation
            if(holdingSwitch){
                if(currentAction == Action::WALKING){
                    sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(3));
                }
                else if(currentAction == Action::IDLE){
                    sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(1));
                }
            }
            else{
                if(currentAction == Action::WALKING){
                    sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(0));
                }
                else if(currentAction == Action::IDLE){
                    sfShape->setTexture(farmerMapPtr[currentAction].sprites.at(0));
                }
            }



            body->SetLinearVelocity(b2Vec2(0, 0));
            break;
        }
        case ID::FARMER:
            break;
        case ID::ALPACA: {

            if (std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(), contactEntity) == currentlyTouchingEntities.end()) {
                currentlyTouchingEntities.push_back(contactEntity);
                // todo: Add farmerTouch to entity?
                auto contactAlpaca = dynamic_cast<Alpaca *> (contactEntity);
                contactAlpaca->farmerTouch = true;
                sfShape->setOutlineColor(sf::Color::Green);

                std::cout << "Touching " << currentlyTouchingEntities.size() << " entities." << std::endl;
            }
        }

        case ID::WOLF:{
            break;
        }
    }
}
