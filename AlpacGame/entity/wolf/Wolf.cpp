#include "Wolf.h"
#include "../trap/Trap.h"

Wolf::Wolf(ConfigGame *configGame, float radius, float width, float height, float x, float y)
        : id(nextId++), Mob(id) {

    /// Assign Pointers
    this->configGame = configGame;

    /// Set ID
    setEntity_ID(Entity::ID::WOLF);

    /// Set in-game values
    // Set HP
    HP = 1;

    // Convert angle and store unit vectors
    convertAngleToVectors(((int) Action::WALKING), walkAngle);

    /// Create Box2D body
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = this;
    body = configGame->world->CreateBody(&bodyDef);

    /// Create Box2D fixtures
    // Fixture: Body
    b2CircleShape b2Shape;
    b2Shape.m_radius = radius / SCALE;
    b2FixtureDef fixtureDef_body;
    fixtureDef_body.shape = &b2Shape;
    fixtureDef_body.density = density;
    fixtureDef_body.friction = friction;
    fixtureDef_body.restitution = restitution;
    fixtureDef_body.filter.categoryBits = (uint16) getEntity_ID();
    fixtureDef_body.filter.maskBits = (uint16) ID::PLANET;
    fixtureDef_body.userData = convertToVoidPtr((int) CollisionID::BODY);
    fixture_body = body->CreateFixture(&fixtureDef_body);

    // Fixture: Hit
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = (radius + 10) / SCALE;
    b2FixtureDef fixtureDef_hit;
    fixtureDef_hit.shape = &b2Shape2;
    fixtureDef_hit.isSensor = true;
    fixtureDef_hit.filter.categoryBits = (uint16) getEntity_ID();
    fixtureDef_hit.filter.maskBits =
            (uint16) ID::FARMER | (uint16) ID::ALPACA | (uint16) ID::TRAP | (uint16) ID::BULLET;
    fixtureDef_hit.userData = convertToVoidPtr((int) CollisionID::HIT);
    fixture_hit = body->CreateFixture(&fixtureDef_hit);

    // Fixture: Detection
    b2CircleShape b2Shape3;
    b2FixtureDef fixtureDef_detection;
    b2Shape3.m_radius = (radius + 300) / SCALE;
    fixtureDef_detection.shape = &b2Shape3;
    fixtureDef_detection.isSensor = true;
    fixtureDef_detection.filter.categoryBits = (uint16) getEntity_ID();
    fixtureDef_detection.filter.maskBits = (uint16) ID::ALPACA | (uint16) ID::FARMER;
    fixtureDef_detection.userData = convertToVoidPtr((int) CollisionID::DETECTION);
    fixture_detection = body->CreateFixture(&fixtureDef_detection);

    /// Create SFML shapes
    // Assign texture map
    wolfTextureMap = configGame->wolfSprites;

    // Shape: Entity
    sf_ShapeEntity = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_ShapeEntity->setOrigin(width / 2, height / 2);

    // Shape: Ghost
    sf_ShapeGhost = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_ShapeGhost->setOrigin(width / 2, height / 2);
    sf_ShapeGhost->setTexture(wolfTextureMap[Action::WALKING][0]);
    sf_ShapeGhost->setFillColor(sf::Color(200, 200, 200, 100));

    // Debug Shape: Body
    sf_DebugBody = new sf::CircleShape(radius);
    sf_DebugBody->setOrigin(radius, radius);
    sf_DebugBody->setFillColor(sf::Color::Transparent);

    // Debug Shape: Hit
    sf_DebugHit = new sf::CircleShape(fixture_hit->GetShape()->m_radius * SCALE);
    sf_DebugHit->setOrigin(sf_DebugHit->getRadius(), sf_DebugHit->getRadius());
    sf_DebugHit->setFillColor(sf::Color::Transparent);

    // Debug Shape: Detection
    sf_DebugDetection = new sf::CircleShape(radius + 300);
    sf_DebugDetection->setOrigin(sf_DebugDetection->getRadius(), sf_DebugDetection->getRadius());
    sf_DebugDetection->setFillColor(sf::Color::Transparent);

    // Debug text: ID
    createLabel(label_ID, &this->configGame->fontID, std::to_string(id));

    // Debug text: HP
    createLabel(label_HP, &this->configGame->fontID, std::to_string(HP));

    // todo: Determine where to put this chunk of code
    /// Initialize behavior
    currentBehavior = Behavior::NORMAL;
    wolfBase = new sf::CircleShape(10);
    wolfBase->setFillColor(sf::Color::Transparent);
    wolfBase->setOrigin(10, 10);
    wolfBase->setPosition(0, 0);


}

int Wolf::nextId = 0;

void Wolf::switchAction() {

    handleHealth();

    if (currentHealth != Health::ALIVE)
        return;

    if (isStunned) {
        return;
    }

    // Check if it is time for randomizing the wolf's current state
    if (currentBehavior == Behavior::NORMAL && randomActionTriggered(randomActionTick)) {

        currentAction = (Action) randomNumberGenerator(0, 1);
        switch (currentAction) {
            case Action::WALKING: {
                currentDirection = (Direction) randomNumberGenerator(0, 1);
                switch (currentDirection) {
                    case Wolf::Direction::LEFT: {
                        sf_ShapeEntity->setScale(-1.f, 1.f);
                        break;
                    }
                    case Wolf::Direction::RIGHT: {
                        sf_ShapeEntity->setScale(1.f, 1.f);
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

    // todo: Remove when behavior is done.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        currentDirection = (Direction) randomNumberGenerator(0, 1);
        switch (currentDirection) {
            case Wolf::Direction::LEFT: {
                sf_ShapeEntity->setScale(-1.f, 1.f);
                break;
            }
            case Wolf::Direction::RIGHT: {
                sf_ShapeEntity->setScale(1.f, 1.f);
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

    if (currentHealth != Health::ALIVE)
        return;

    if (isStunned) {
        return;
    }

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
    float delta_Y = sf_ShapeEntity->getLocalBounds().height / 2 - fixture_body->GetShape()->m_radius * SCALE;
    b2Vec2 offsetPoint = body->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

    float shape_x = offsetPoint.x * SCALE;
    float shape_y = offsetPoint.y * SCALE;

    sf_ShapeEntity->setPosition(shape_x, shape_y);
    sf_ShapeEntity->setRotation((body->GetAngle() * DEGtoRAD));

    // Switch Texture
    switch (currentHealth) {
        case Health::ALIVE: {

            // Switch Texture
            switch (currentStatus) {
                case Status::GROUNDED: {
                    if (currentAction == Action::IDLE) {
                        sf_ShapeEntity->setTexture(wolfTextureMap[Action::IDLE][0]);
                    } else {
                        sf_ShapeEntity->setTexture(wolfTextureMap[Action::WALKING][(spriteSwitch ? 0 : 1)]);
                    }
                    break;
                }
                case Status::AIRBORNE: {
                    sf_ShapeEntity->setTexture(wolfTextureMap[Action::WALKING][(spriteSwitch ? 0 : 1)]);
                    break;
                }
            }
            break;

        }
        case Health::DEAD: {
            sf_ShapeEntity->setTexture(wolfTextureMap[Action::IDLE][3]);
            break;
        }
        case Health::GHOST: {
            if (deathClock.getElapsedTime().asSeconds() >= decayTick) {
                sf_ShapeEntity->setFillColor(sf_ShapeEntity->getFillColor() - sf::Color(0, 0, 0, 1));
            }
            break;
        }
    }

    if (currentHealth == Health::GHOST) {
        b2Vec2 ghostMovementVector = getBody()->GetWorldVector(b2Vec2(0.f, -0.01f));
        sf_ShapeGhost->move(sf::Vector2f(ghostMovementVector.x * SCALE, ghostMovementVector.y * SCALE));
        window->draw(*sf_ShapeGhost);
    }

    window->draw(*sf_ShapeEntity);

    if (configGame->showLabels) {

        /// Draw SFShape debug
        sf_ShapeEntity->setOutlineThickness(2);
        sf_ShapeEntity->setOutlineColor(sf::Color::Black);

        /// Draw Base THIS SHOULD BE FIXED
        wolfBase->setOutlineThickness(2);
        wolfBase->setOutlineColor(sf::Color::Cyan);
        if (wolfBase->getPosition().x == 0)
            wolfBase->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        window->draw(*wolfBase);

        sf_DebugBody->setOutlineThickness(2);
        sf_DebugBody->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugBody->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_DebugBody);

        /// Detect Sensor Draw
        sf_DebugDetection->setOutlineThickness(2);
        sf_DebugDetection->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        if (currentBehavior == Behavior::NORMAL) sf_DebugDetection->setOutlineColor(sf::Color::Yellow);
        else if (currentBehavior == Behavior::HUNTING) sf_DebugDetection->setOutlineColor(sf::Color::Red);
        else if (currentBehavior == Behavior::AFRAID) sf_DebugDetection->setOutlineColor(sf::Color::Green);
        window->draw(*sf_DebugDetection);

        // Draw hitSensor debug
        sf_DebugHit->setOutlineThickness(2);
        sf_DebugHit->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugHit->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_DebugHit);

        // Draw label_ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID->setPosition(body->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              body->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(sf_ShapeEntity->getRotation());
        window->draw(*label_ID);

        // Draw label_HP
        label_HP->setString(std::to_string(HP));
        label_HP->setPosition(getBody()->GetWorldCenter().x * SCALE, getBody()->GetWorldCenter().y * SCALE);
        label_HP->setRotation(sf_ShapeEntity->getRotation());
        window->draw(*label_HP);


    } else {
        sf_ShapeEntity->setOutlineThickness(0);
    }
}

bool Wolf::deadCheck() {
    return currentHealth == Health::GHOST && !deathClock.isRunning();
}

Wolf::~Wolf() {
    printf("Wolf %i died.\n", id);
    isStunned = false;

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
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            currentStatus = Status::GROUNDED;
            spriteSwitch = !spriteSwitch;
            sf_DebugHit->setOutlineColor(sf::Color::White);
            body->SetLinearVelocity(b2Vec2(0, 0));
            break;
        }
        default:
            break;
    }
}

void Wolf::startContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::FARMER: {
            if (currentHealth == Health::ALIVE
                && !isStunned
                && otherCollision == CollisionID::HIT) {

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

                dynamic_cast<EntityWarm *>(contactEntity)->dealDamage(1);
            }
            break;

        }
        case ID::ALPACA: {

            if (currentHealth == Health::ALIVE
                && !isStunned
                && otherCollision == CollisionID::HIT) {

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

                dynamic_cast<EntityWarm *>(contactEntity)->dealDamage(1);

            }
            break;
        }
        default:
            break;
    }
}

void Wolf::startContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::FARMER:
            break;
        case ID::ALPACA: {
            /// All sensor his
            if (!isStunned && otherCollision == CollisionID::HIT) {
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
                if (delta.x > 0) {
                    currentDirection = Direction::RIGHT;
                    sf_ShapeEntity->setScale(1.f, 1.f);

                } else {
                    currentDirection = Direction::LEFT;
                    sf_ShapeEntity->setScale(-1.f, 1.f);
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
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            currentStatus = Status::AIRBORNE;
            sf_DebugHit->setOutlineColor(sf::Color(100, 100, 100));
            break;
        }
        default:
            break;
    }
}

void Wolf::endContact_hit(Entity::CollisionID otherCollision, Entity *contactEntity) {

}

void Wolf::endContact_detection(Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::FARMER:
            break;
        case ID::ALPACA: {
            if (otherCollision == CollisionID::HIT) {
                b2Vec2 delta = getBody()->GetLocalPoint(contactEntity->getBody()->GetWorldCenter());
                if (delta.x > 0) {
                    currentDirection = Direction::RIGHT;
                    sf_ShapeEntity->setScale(1.f, 1.f);

                } else {
                    currentDirection = Direction::LEFT;
                    sf_ShapeEntity->setScale(-1.f, 1.f);
                }
            }
            break;
        }
        default:
            break;
    }
}

void Wolf::performStun() {

    printf("Wolf %i is stunned!\n", id);

    // Set bool
    isStunned = true;

    // Switch OFF certain collisions
    b2Filter tempFilter = fixture_hit->GetFilterData();
    tempFilter.maskBits ^= (uint16) ID::FARMER | (uint16) ID::ALPACA;
    fixture_hit->SetFilterData(tempFilter);

    // Sleep body
    getBody()->SetAwake(false);

    // Reset values
    currentAction = Action::IDLE;
}

void Wolf::removeStun() {

    printf("Wolf %i is no longer stunned!\n", id);

    // Set bool
    isStunned = false;

    // Switch ON certain collisions
    b2Filter tempFilter = fixture_hit->GetFilterData();
    tempFilter.maskBits |= (uint16) ID::FARMER | (uint16) ID::ALPACA;
    fixture_hit->SetFilterData(tempFilter);

    // Wake body
    getBody()->SetAwake(true);

    // Reset values
    getBody()->SetLinearVelocity(b2Vec2(0, 0));
}

