#include "Wolf.h"

Wolf::Wolf(ConfigGame *configGame, float radius, float width, float height, float x, float y)
        : id(nextId++), Mob(id) {

    /// Assign Pointers
    this->configGame = configGame;

    /// Define entity info
    // Set entity ID
    setEntity_ID(Entity::ID::WOLF);

    // Set HP
    HP = 10;

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
    fixtureDef_body.filter.maskBits = (uint16) ID::PLANET | (uint16) ID::BULLET;
    fixtureDef_body.userData = convertToVoidPtr((int) CollisionID::BODY);
    fixture_body = body->CreateFixture(&fixtureDef_body);

    // Fixture: Hit
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = (radius) / SCALE;
    b2FixtureDef fixtureDef_hit;
    fixtureDef_hit.shape = &b2Shape2;
    fixtureDef_hit.isSensor = true;
    fixtureDef_hit.filter.categoryBits = (uint16) getEntity_ID();
    fixtureDef_hit.filter.maskBits =
            (uint16) ID::FARMER | (uint16) ID::ALPACA | (uint16) ID::TRAP;
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
    /// WolfBase
    // Wolf Den is placed on an angle with planets radius.
    // 180 degrees because farmer position is at 0 degrees.
    wolfDen_Debug = new sf::CircleShape(10);
    wolfDen_Debug->setFillColor(sf::Color::Transparent);
    wolfDen_Debug->setOrigin(10, 10);
    float tmp_X = configGame->calcX(180.f, configGame->planetRadius);
    float tmp_Y = configGame->calcY(180.f, configGame->planetRadius);

    wolfDen_Debug->setPosition( tmp_X, tmp_Y);
    wolfDenVec2 = b2Vec2(tmp_X / SCALE, tmp_Y / SCALE);

    currentBehavior = Behavior::HUNTING;
    randomActionClock.reset(true);
    movementTriggerClock.reset(true);

}

int Wolf::nextId = 0;

void Wolf::switchAction() {
    // Handle the health logic
    handleHealth();

    // Cancel Early if entity is not alive or stunned
    if (currentHealth != Health::ALIVE || isStunned)
        return;

    // TODO: Hunting when wolves spawn at night. Temporarily is set to input "M"
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
        currentAction = Action::WALKING;
        currentBehavior = Behavior ::HUNTING;

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
    }

    // TODO: Wolves runs home when night is over. Temporarily set to input "N"
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
        currentBehavior = Behavior::RUNHOME;
    }

    // Changes to the direction, nearest to WolfDen
    if (currentBehavior == Behavior::RUNHOME) {
        currentAction = Action::WALKING;
        b2Vec2 test = getBody()->GetLocalPoint(wolfDenVec2);

        if(test.x < 0){
            currentDirection = Direction::LEFT;
            sf_ShapeEntity->setScale(-1.f, 1.f);
        } else {
            currentDirection = Direction::RIGHT;
            sf_ShapeEntity->setScale(1.f, 1.f);
        }
    }


}

void Wolf::performAction() {

    // Cancel Early if entity is not alive or stunned
    if (currentHealth != Health::ALIVE || isStunned)
        return;

    // Check if the entity is allowed to move
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


    // Wolves go back to Den, then disappears when Length() is a small number
    // TODO: When arrives at den, make them idle. then fades away.
    // Temporary makes them disappear.
    if(currentBehavior == Behavior::RUNHOME){
        b2Vec2 delta = body->GetLocalPoint(wolfDenVec2);
        float tmp = delta.Length();

        if(tmp < 3.0f && currentBehavior == Behavior::RUNHOME){
            removeEntity();
            currentAction = Action ::IDLE;
            currentBehavior = Behavior ::ENTERHOME;
            behaviorClock.reset(true);

        }

    }


    /** HUNTING
     *  1. Moves to one direction (Random) until an entity is detected
     *  2. Adds the entity in a list<Entity *>
     *  3. Iterates list, finds the lowest Length() value and follows it
     */

    if (currentBehavior == Behavior::HUNTING) {
        currentAction = Action::WALKING;

        if(!currentlyDetectedEntities.empty()) {

            b2Vec2 tmp;
            // Reset b2Vec to its corresponding direction
            if(currentDirection == Direction::RIGHT) tmp = b2Vec2( 10, 10);
            else                                     tmp = b2Vec2(-10, 10);

            for (auto &detectedEntity : currentlyDetectedEntities) {

                b2Vec2 currentVector = body->GetLocalPoint(detectedEntity->getBody()->GetWorldCenter());
                float length_1 = currentVector.LengthSquared();
                float length_2 = tmp.LengthSquared();

                if(length_1 < length_2){
                    tmp = currentVector;
                }

            }
            if (tmp.x < 0) {
                currentDirection = Direction::LEFT;
                sf_ShapeEntity->setScale(-1.f, 1.f);
            } else {
                currentDirection = Direction::RIGHT;
                sf_ShapeEntity->setScale(1.f, 1.f);
            }

        }
    }

    if(currentBehavior == Behavior::ENTERHOME && behaviorClock.getElapsedTime().asSeconds() >  1.0f){
        currentHealth = Health ::GHOST;
        deathClock.reset(false);
    }
}

void Wolf::render(sf::RenderWindow *window) {

    // Switch textures
    switchCurrentTexture();

    // Calculate the position and rotation of shape of entity
    calcShapeEntityPlacement();

    // Handle death rendering
    if (currentHealth == Health::GHOST){
        renderDeath();
        window->draw(*sf_ShapeGhost);
    }
    if(currentBehavior == Behavior::ENTERHOME){
        renderFadeOut();
    }

    // Draw entity shape
    window->draw(*sf_ShapeEntity);

    // Render debug if activated
    renderDebugMode();

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

            /// Placing Farmer to Vector
            if(otherCollision == CollisionID::HIT){
                currentlyDetectedEntities.push_back(contactEntity);
            }

            break;
        case ID::ALPACA: {

            /// Placing Alpaca to Vector
            if(otherCollision == CollisionID::HIT){
                currentlyDetectedEntities.push_back(contactEntity);
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
            /// Removing Farmer from Vector
            if(otherCollision == CollisionID::HIT){
                currentlyDetectedEntities.remove(contactEntity);
            }

            break;
        case ID::ALPACA: {

            /// Removing Alpaca from Vector
            if(otherCollision == CollisionID::HIT){
                currentlyDetectedEntities.remove(contactEntity);
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

    // Sleep body
    getBody()->SetAwake(false);

    // Reset values
    currentAction = Action::IDLE;
}

void Wolf::removeStun() {

    printf("Wolf %i is no longer stunned!\n", id);

    // Set bool
    isStunned = false;

    // Wake body
    getBody()->SetAwake(true);

    // Reset values
    getBody()->SetLinearVelocity(b2Vec2(0, 0));
}

void Wolf::switchCurrentTexture() {
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

            break;
        }
    }

}

void Wolf::renderDebugMode() {

    if (configGame->showDebugMode) {

        // Draw debug: Entity Shape
        sf_ShapeEntity->setOutlineThickness(2);
        sf_ShapeEntity->setOutlineColor(sf::Color::Black);

        // Draw debug: Body
        sf_DebugBody->setOutlineThickness(2);
        sf_DebugBody->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugBody->setRotation(body->GetAngle() * DEGtoRAD);
        configGame->window->draw(*sf_DebugBody);

        // Draw debug: Hit
        sf_DebugHit->setOutlineThickness(2);
        sf_DebugHit->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugHit->setRotation(body->GetAngle() * DEGtoRAD);
        configGame->window->draw(*sf_DebugHit);

        // Draw debug: Detection
        sf_DebugDetection->setOutlineThickness(2);
        sf_DebugDetection->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        if (currentBehavior == Behavior::ENTERHOME) sf_DebugDetection->setOutlineColor(sf::Color::Yellow);
        else if (currentBehavior == Behavior::HUNTING) sf_DebugDetection->setOutlineColor(sf::Color::Red);
        else if (currentBehavior == Behavior::RUNHOME) sf_DebugDetection->setOutlineColor(sf::Color::Green);
        configGame->window->draw(*sf_DebugDetection);

        // Draw debug: Label ID
        float offset = fixture_body->GetShape()->m_radius + 1.f;
        label_ID->setPosition(getBody()->GetWorldPoint(b2Vec2(0, -offset)).x * SCALE,
                              getBody()->GetWorldPoint(b2Vec2(0, -offset)).y * SCALE);
        label_ID->setRotation(sf_ShapeEntity->getRotation());
        configGame->window->draw(*label_ID);

        // Draw debug: Label HP
        label_HP->setString(std::to_string(HP));
        label_HP->setPosition(getBody()->GetWorldCenter().x * SCALE,
                              getBody()->GetWorldCenter().y * SCALE);
        label_HP->setRotation(sf_ShapeEntity->getRotation());
        configGame->window->draw(*label_HP);

        // todo: Fix this
        // Draw debug: Base
        wolfDen_Debug->setOutlineThickness(2);
        wolfDen_Debug->setOutlineColor(sf::Color::Cyan);
        configGame->window->draw(*wolfDen_Debug);


    } else {
        sf_ShapeEntity->setOutlineThickness(0);
    }
}

void Wolf::pause() {
    movementTriggerClock.pause();
    randomActionClock.pause();
    behaviorClock.pause();
    deathClock.pause();
}

void Wolf::resume() {
    movementTriggerClock.resume();
    randomActionClock.resume();
    behaviorClock.resume();

    if(currentHealth != Health::ALIVE){
        deathClock.resume();
    }
}

