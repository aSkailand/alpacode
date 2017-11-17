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

    /// Body Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / SCALE;
    b2FixtureDef bodySensor;
    bodySensor.shape = &b2Shape2;
    bodySensor.isSensor = true;
    bodySensor.filter.categoryBits = (uint16) ID::WOLF;
    bodySensor.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::ALPACA;

    /// Detect Sensor
    b2CircleShape b2Shape3;
    b2FixtureDef detectSensor;
    b2Shape3.m_radius = (radius + 300) / SCALE;
    detectSensor.shape = &b2Shape3;
    detectSensor.isSensor = true;
    detectSensor.filter.categoryBits = (uint16) ID::WOLF;
    detectSensor.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::FARMER;


    // Store information
    setID(Entity::ID::WOLF);
    body->SetUserData((void *) this);

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    bodySensorFixture = body->CreateFixture(&bodySensor);
    detectSensorFixture = body->CreateFixture(&detectSensor);

    // Set Hit and Detect Fixture as Sensor
    bodyFixture->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    bodySensorFixture->SetUserData(convertToVoidPtr((int) CollisionID::HIT));
    detectSensorFixture->SetUserData(convertToVoidPtr((int) CollisionID::DETECTION));



    // Create SFML shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setTexture(&configGame->wolfTexture);

    // Create ID text
    createLabel(std::to_string(id), &this->configGame->fontID);

    /// Initialize behavior
    currentBehavior = Behavior::NORMAL;
    wolfBase = new sf::CircleShape(10);
    wolfBase->setFillColor(sf::Color::Transparent);
    wolfBase->setOrigin(10, 10);
    wolfBase->setPosition(0 ,0 );


    ///Paint Sensor;
    sf_DetectSensor = new sf::CircleShape(radius + 300);
    sf_DetectSensor->setFillColor(sf::Color::Transparent);
    sf_DetectSensor->setOrigin(radius+300, radius+300);



}

int Wolf::nextId = 0;

void Wolf::switchAction() {

    // Check if it is time for randomizing the wolf's current state
    if (currentBehavior == Behavior ::NORMAL &&
            randomActionTriggered(randomActionTick)) {

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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)){
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

        currentBehavior = Behavior ::HUNTING;
        std::cout << "Hunting" << std::endl;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
        currentBehavior = Behavior::AFRAID;


    }


    if(currentBehavior == Behavior::HUNTING){
        currentAction = Action::WALKING;
    }

    if(currentBehavior == Behavior::AFRAID){
       //std::cout << "Afraid, Going Home" << std::endl;

        currentAction = Action::WALKING;


    }


}


void Wolf::render(sf::RenderWindow *window) {


    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;



    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);

    if (configGame->showLabels) {
        /// Draw Base THIS SHOULD BE FIXED
        wolfBase->setOutlineThickness(2);
        wolfBase->setOutlineColor(sf::Color::Cyan);
        if(wolfBase->getPosition().x == 0 ) wolfBase->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        window->draw(*wolfBase);

        /// Detect Sensor Draw
        sf_DetectSensor->setOutlineThickness(2);
        sf_DetectSensor->setPosition(body->GetPosition().x *SCALE, body->GetPosition().y * SCALE);
        if(currentBehavior == Behavior::NORMAL) sf_DetectSensor->setOutlineColor(sf::Color::Yellow);
        else if(currentBehavior == Behavior::HUNTING) sf_DetectSensor->setOutlineColor(sf::Color::Red);
        else if(currentBehavior == Behavior::AFRAID) sf_DetectSensor->setOutlineColor(sf::Color::Green);
        window->draw(*sf_DetectSensor);


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

void Wolf::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET:

            currentStatus = Status ::GROUNDED;
            body->SetLinearVelocity(b2Vec2(0,0));
            break;
        case ID::FARMER: {
            if(selfCollision == CollisionID ::DETECTION) break;

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


            if(selfCollision == CollisionID::HIT){

                if(otherCollision == CollisionID::HIT){

                    /// Self sensor hits contact sensor
                    float mass = contactEntity->getBody()->GetMass();
                    float tempAngle = attackAngle;

                    if(delta.x > 0){ /// Hit from the right side, force must be going left
                        tempAngle = 180 - tempAngle; }

                    b2Vec2 dir = contactEntity->getBody()->GetWorldVector(b2Vec2( -cos(tempAngle *b2_pi/180), -sin( tempAngle* b2_pi/180)));

                    contactEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                    contactEntity->getBody()->ApplyLinearImpulseToCenter(mass* attackForce * dir, true);
                }

            }
            if(selfCollision == CollisionID::DETECTION){
                /// All sensor his
                if(otherCollision == CollisionID::HIT){
                    if(delta.x > 0){
                        currentDirection = Direction::RIGHT;
                        sfShape->setScale(1.f, 1.f);

                    } else {
                        currentDirection = Direction ::LEFT;
                        sfShape->setScale(-1.f, 1.f);
                    }

                }
            }




            break;
        }

        case ID::WOLF:
            break;
    }
}

void Wolf::endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()){
        case ID::PLANET:{
            currentStatus = Status::AIRBORNE;

            break;
        }
        case ID::ALPACA:{
            b2Vec2 delta =  getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
            if(otherCollision == CollisionID::DETECTION) break;

            if(delta.x > 0){
                currentDirection = Direction::RIGHT;
                sfShape->setScale(1.f, 1.f);

            } else {
                currentDirection = Direction ::LEFT;
                sfShape->setScale(-1.f, 1.f);
            }


            break;
        }
        case ID::FARMER:{
            break;
        }

    }



}
