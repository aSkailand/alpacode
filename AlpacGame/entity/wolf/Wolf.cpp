#include "Wolf.h"

Wolf::Wolf(ConfigGame *configGame, float radius, float width, float height, float x, float y)
        : id(nextId++), Mob(id) {

    // Assign Pointers
    this->configGame = configGame;
    wolfMapPtr = configGame->wolfSprites;

    // Convert angle and store unit vectors
    convertAngleToVectors(((int) Action::WALKING), walkAngle);

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = configGame->world->CreateBody(&bodyDef);

    // Create Fixture
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_body;
    fixtureDef_body.density = density;
    fixtureDef_body.friction = friction;
    fixtureDef_body.restitution = restitution;
    fixtureDef_body.filter.categoryBits = categoryBits;
    fixtureDef_body.filter.maskBits = maskBits;
    fixtureDef_body.shape = &b2Shape;

    /// Body Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_hit;
    fixtureDef_hit.shape = &b2Shape2;
    fixtureDef_hit.isSensor = true;
    fixtureDef_hit.filter.categoryBits = (uint16) ID::WOLF;
    fixtureDef_hit.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::ALPACA;

    /// Detect Sensor
    b2CircleShape b2Shape3;
    b2FixtureDef fixtureDef_detection;
    b2Shape3.m_radius = (radius + 300) / SCALE;
    fixtureDef_detection.shape = &b2Shape3;
    fixtureDef_detection.isSensor = true;
    fixtureDef_detection.filter.categoryBits = (uint16) ID::WOLF;
    fixtureDef_detection.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::FARMER;

    // Store information
    setID(Entity::ID::WOLF);
    body->SetUserData((void *) this);

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef_body);
    fixture_hit = body->CreateFixture(&fixtureDef_hit);
    fixture_detection = body->CreateFixture(&fixtureDef_detection);

    // Set Hit and Detect Fixture as Sensor
    fixture_body->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    fixture_hit->SetUserData(convertToVoidPtr((int) CollisionID::HIT));
    fixture_detection->SetUserData(convertToVoidPtr((int) CollisionID::DETECTION));

    // Create SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width / 2, height / 2);

    sf_HitSensor = new sf::CircleShape(radius);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(radius, radius);

    // Set HP
    HP = 10;

    // Create ID text
    createLabel(label_ID, &this->configGame->fontID, std::to_string(id));
    createLabel(label_HP, &this->configGame->fontID, std::to_string(HP));

    /// Initialize behavior
    currentBehavior = Behavior::NORMAL;
    wolfBase = new sf::CircleShape(10);
    wolfBase->setFillColor(sf::Color::Transparent);
    wolfBase->setOrigin(10, 10);
    wolfBase->setPosition(0, 0);


    ///Paint Sensor;
    sf_DetectSensor = new sf::CircleShape(radius + 300);
    sf_DetectSensor->setFillColor(sf::Color::Transparent);
    sf_DetectSensor->setOrigin(radius + 300, radius + 300);


}

int Wolf::nextId = 0;

void Wolf::switchAction() {

//    if(!alive)
//        return;

    // Check if it is time for randomizing the wolf's current state
    if (randomActionTriggered(randomActionTick) && currentBehavior == Behavior::NORMAL) {

        currentAction = (Action) randomNumberGenerator(0, 1);
        switch (currentAction) {
            case Action::WALKING: {
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
            case Action::IDLE: {
                break;
            }

        }

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
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

        currentBehavior = Behavior::HUNTING;
        std::cout << "Hunting" << std::endl;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
        currentBehavior = Behavior::AFRAID;
    }

    if (currentBehavior == Behavior::HUNTING) {
        currentAction = Action::WALKING;
    }

    if (currentBehavior == Behavior::AFRAID) {
        currentAction = Action::WALKING;
    }


}


void Wolf::performAction() {

    if(!alive)
        return;

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

void Wolf::render(sf::RenderWindow *window) {

    /// Render sfShape
    float delta_Y = sfShape->getLocalBounds().height / 2 - fixture_body->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    // Switch Texture
    switch (currentStatus) {
        case Status::GROUNDED: {
            if (currentAction == Action::IDLE) {
                sfShape->setTexture(wolfMapPtr[Action::IDLE][0]);
            } else {
                sfShape->setTexture(wolfMapPtr[Action::WALKING][(spriteSwitch ? 0 : 1)]);
            }
            break;
        }
        case Status::AIRBORNE: {
            sfShape->setTexture(wolfMapPtr[Action::WALKING][(spriteSwitch ? 0 : 1)]);
            break;
        }
    }

    window->draw(*sfShape);

    if (configGame->showLabels) {

        /// Draw SFShape debug
        sfShape->setOutlineThickness(2);
        sfShape->setOutlineColor(sf::Color::Black);

        /// Draw Base THIS SHOULD BE FIXED
        wolfBase->setOutlineThickness(2);
        wolfBase->setOutlineColor(sf::Color::Cyan);
        if (wolfBase->getPosition().x == 0)
            wolfBase->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        window->draw(*wolfBase);

        /// Detect Sensor Draw
        sf_DetectSensor->setOutlineThickness(2);
        sf_DetectSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        if (currentBehavior == Behavior::NORMAL) sf_DetectSensor->setOutlineColor(sf::Color::Yellow);
        else if (currentBehavior == Behavior::HUNTING) sf_DetectSensor->setOutlineColor(sf::Color::Red);
        else if (currentBehavior == Behavior::AFRAID) sf_DetectSensor->setOutlineColor(sf::Color::Green);
        window->draw(*sf_DetectSensor);

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

        // Draw label_ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(sfShape->getRotation());
        window->draw(*label_ID);

        // Draw label_HP
        label_HP->setString(std::to_string(HP));
        label_HP->setPosition(getBody()->GetWorldCenter().x * SCALE, getBody()->GetWorldCenter().y * SCALE);
        label_HP->setRotation(sfShape->getRotation());
        window->draw(*label_HP);


    } else {
        sfShape->setOutlineThickness(0);
    }
}

bool Wolf::deadCheck() {
    return !alive && deathClock.getElapsedTime().asSeconds() >= deathTick;
}

Wolf::~Wolf() {
    printf("Wolf %i died.\n", id);
}

void Wolf::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {

    switch (selfCollision) {
        case CollisionID::BODY: {
            startContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::HIT: {
            startContact_hit(otherCollision, contactEntity);
            break;
        }
        case CollisionID::DETECTION: {
            startContact_detection(otherCollision, contactEntity);
            break;
        }
    }

}

void Wolf::startContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            spriteSwitch = !spriteSwitch;
            sf_HitSensor->setOutlineColor(sf::Color::White);
            body->SetLinearVelocity(b2Vec2(0, 0));
            body->SetAwake(false);
            break;
        }
        default:
            break;
    }
}

void Wolf::startContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::FARMER: {
            if (otherCollision == CollisionID::HIT) {
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());

                /// Self sensor hits contact sensor
                float mass = contactEntity->getBody()->GetMass();
                float tempAngle = attackAngle;

                if (delta.x > 0) { /// Hit from the right side, force must be going left
                    tempAngle = 180 - tempAngle;
                }

                b2Vec2 dir = contactEntity->getBody()->GetWorldVector(
                        b2Vec2(-cos(tempAngle * b2_pi / 180), -sin(tempAngle * b2_pi / 180)));

                contactEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                contactEntity->getBody()->ApplyLinearImpulseToCenter(mass * attackForce * dir, true);

                static_cast<EntityWarm*>(contactEntity)->dealDamage(1);
            }
            break;

        }
        case ID::ALPACA: {

            if (otherCollision == CollisionID::HIT) {
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
                /// Self sensor hits contact sensor
                float mass = contactEntity->getBody()->GetMass();
                float tempAngle = attackAngle;

                if (delta.x > 0) { /// Hit from the right side, force must be going left
                    tempAngle = 180 - tempAngle;
                }

                b2Vec2 dir = contactEntity->getBody()->GetWorldVector(
                        b2Vec2(-cos(tempAngle * b2_pi / 180), -sin(tempAngle * b2_pi / 180)));

                contactEntity->getBody()->SetLinearVelocity(b2Vec2(0, 0));
                contactEntity->getBody()->ApplyLinearImpulseToCenter(mass * attackForce * dir, true);

                static_cast<EntityWarm*>(contactEntity)->dealDamage(1);

            }
            break;
        }
        default:
            break;
    }
}

void Wolf::startContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::FARMER:
            break;
        case ID::ALPACA: {
            /// All sensor his
            if (otherCollision == CollisionID::HIT) {
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
                if (delta.x > 0) {
                    currentDirection = Direction::RIGHT;
                    sfShape->setScale(1.f, 1.f);

                } else {
                    currentDirection = Direction::LEFT;
                    sfShape->setScale(-1.f, 1.f);
                }
            }
            break;
        }
        default:
            break;
    }
}

void Wolf::endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {
    switch (selfCollision) {
        case CollisionID::BODY: {
            endContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::HIT: {
            endContact_body(otherCollision, contactEntity);
            break;
        }
        case CollisionID::DETECTION: {
            endContact_detection(otherCollision, contactEntity);
            break;
        }
    }
}

void Wolf::endContact_body(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            sf_HitSensor->setOutlineColor(sf::Color(100, 100, 100));
            break;
        }
        default:
            break;
    }
}

void Wolf::endContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {

}

void Wolf::endContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch(contactEntity->getID()){
        case ID::FARMER:break;
        case ID::ALPACA:{
            if(otherCollision == CollisionID::HIT){
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
                if (delta.x > 0) {
                    currentDirection = Direction::RIGHT;
                    sfShape->setScale(1.f, 1.f);

                } else {
                    currentDirection = Direction::LEFT;
                    sfShape->setScale(-1.f, 1.f);
                }
            }
            break;
        }
        default:
            break;
    }
}

void Wolf::initDeath() {
    printf("Wolf %i is dieing.\n", id);
    b2Filter deadFilter;
    deadFilter.categoryBits = (uint16) ID::WOLF;
    deadFilter.maskBits = (uint16) ID::PLANET;
    fixture_body->SetFilterData(deadFilter);
    fixture_hit->SetFilterData(deadFilter);
    fixture_detection->SetFilterData(deadFilter);

    sfShape->setFillColor(sf::Color(200,200,200,100));
    deathClock.reset(true);
}
