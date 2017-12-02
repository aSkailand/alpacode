
#include "Trap.h"

Trap::Trap(ConfigGame *configGame, float length, float height, float x, float y) {
    this->configGame = configGame;

    // Create Body
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;
    body = configGame->world->CreateBody(&bodyDef);

    // Create Body
    b2PolygonShape b2shape;
    b2shape.SetAsBox(length / 2 / SCALE, height / 2 / SCALE);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.5f;
    fixtureDef.shape = &b2shape;
    fixtureDef.filter.categoryBits = (uint16) ID::TRAP;
    fixtureDef.filter.maskBits = (uint16) ID::PLANET;

    // Create Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = ((length + 50) / 2) / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::TRAP;
    sensor.filter.maskBits = (uint16) ID::WOLF | (uint16) ID::FARMER;

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef);
    fixture_hit = body->CreateFixture(&sensor);

    // Set Fixture Sense to given Enums
    fixture_body->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    fixture_hit->SetUserData(convertToVoidPtr((int) CollisionID::HIT));

    // Store Information
    setEntity_ID(Entity::ID::TRAP);
    body->SetUserData((void *) this);

    // Create SFML shape
    sf_ShapeEntity = new sf::RectangleShape(sf::Vector2f(length, height));
    sf_ShapeEntity->setOrigin(length / 2, height / 2);

    // Create shape for sensor
    sf_DebugHit = new sf::CircleShape(fixture_hit->GetShape()->m_radius * SCALE);
    sf_DebugHit->setFillColor(sf::Color::Transparent);
    sf_DebugHit->setOrigin(fixture_hit->GetShape()->m_radius * SCALE, fixture_hit->GetShape()->m_radius * SCALE);

    // Create barometer
    trapCooldownBarometer = new CooldownBarometer(configGame->cooldownTextures, &trapClock, 30.f, 30.f);

}

void Trap::render(sf::RenderWindow *window) {

    switch (currentMode) {
        case Mode::CLOSED: {
            sf_DebugHit->setOutlineColor(sf::Color::Black);
            sf_ShapeEntity->setTexture(&configGame->trapClosedTexture);
            break;
        }
        case Mode::OPEN: {
            sf_DebugHit->setOutlineColor(sf::Color::White);
            sf_ShapeEntity->setTexture(&configGame->trapOpenTexture);
            break;
        }
        case Mode::READY: {
            sf_DebugHit->setOutlineColor(sf::Color::Green);
            sf_ShapeEntity->setTexture(&configGame->trapOpenTexture);
            break;
        }
        case Mode::LATCHED: {
            sf_DebugHit->setOutlineColor(sf::Color::Red);
            sf_ShapeEntity->setTexture(&configGame->trapClosedTexture);
            break;

        }
    }

    /// Render sfShape
    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sf_ShapeEntity->setPosition(shape_x, shape_y);
    sf_ShapeEntity->setRotation((body->GetAngle() * DEGtoRAD));


    if (configGame->showLabels) {

        // Draw hitSensor debug
        sf_DebugHit->setOutlineThickness(2);
        sf_DebugHit->setPosition(shape_x, shape_y);
        sf_DebugHit->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_DebugHit);

        sf_ShapeEntity->setOutlineThickness(3);
        sf_ShapeEntity->setOutlineColor(farmerTouch ? sf::Color::Yellow : sf::Color::Black);

    } else {
        sf_ShapeEntity->setOutlineThickness(0);
    }

    window->draw(*sf_ShapeEntity);


    trapCooldownBarometer->setPlacement(
            getBody()->GetWorldPoint(b2Vec2(0.f, -3.f)).x * SCALE,
            getBody()->GetWorldPoint(b2Vec2(0.f, -3.f)).y * SCALE,
            sf_ShapeEntity->getRotation());
    trapCooldownBarometer->render(window);
}

void Trap::startContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            if (selfCollision == Entity::CollisionID::BODY) {
            }
            break;
        }
        case ID::WOLF: {

            if (otherCollision == Entity::CollisionID::HIT && !checkIfTouching(contactEntity)) {

                auto *wolf = dynamic_cast<Wolf *>(contactEntity);
                if (!wolf->isStunned) {
                    currentlyTouchingWolves.push_back(wolf);
                }
            }
            break;
        }
        default: {
            break;
        }

    }

}

void Trap::endContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET: {
            break;
        }
        case ID::WOLF: {
            if (otherCollision == Entity::CollisionID::HIT && checkIfTouching(contactEntity)) {
                currentlyTouchingWolves.remove(dynamic_cast<Wolf *>(contactEntity));
            }

            break;
        }
        default:
            break;
    }

}

bool Trap::deadCheck() {
    return false;
}

void Trap::performHold() {
    getBody()->SetAwake(false);
}

void Trap::performThrow() {
    if (currentMode == Mode::CLOSED) {
        trapClock.reset(false);
    } else if (currentMode == Mode::OPEN) {
        currentMode = Mode::READY;
    }
}

void Trap::update() {

    switch (currentMode) {
        case Mode::CLOSED: {

            // Reset Timer when trap is held and closed
            if (!trapClock.isRunning()) {
                if (isHeld){
                    trapClock.reset(true);
                    trapCooldownBarometer->calcTicks(openTick);
                }
            } else{

                if (trapClock.getElapsedTime().asSeconds() >= openTick) {
                    currentMode = Mode::OPEN;
                    trapClock.reset(false);


                    printf("Open!\n");
                }
            }
            break;
        }
        case Mode::OPEN: {
            break;
        }
        case Mode::READY: {

            // If trap is currently touching wolves
            if (!currentlyTouchingWolves.empty()) {

                // Select target and stun it
                stunnedTarget = currentlyTouchingWolves.front();
                stunnedTarget->performStun();

                // Start latch time
                currentMode = Mode::LATCHED;
                trapCooldownBarometer->calcTicks(stunTick);
                trapClock.reset(true);
            }
            break;
        }
        case Mode::LATCHED: {

            /*
             * Three possible outcomes at any given time when a target is latched:
             *      1. Target is killed before the latching period is over  -> Trap is closed.
             *      2. Target has survived the latching period              -> Trap is closed.
             *      3. Target is still being latched                        -> Trap is latched.
             */

            if (checkIfTargetIsDead()) {

                // Clean trap
                stunnedTarget = nullptr;
                currentMode = Mode::CLOSED;
                trapClock.reset(false);

            } else if (trapClock.getElapsedTime().asSeconds() >= stunTick) {

                // Remove stun on target
                stunnedTarget->removeStun();

                // Clean trap
                stunnedTarget = nullptr;
                currentMode = Mode::CLOSED;
                trapClock.reset(false);

            } else {
                // Move target accordingly to the trap
                b2Vec2 offset = getBody()->GetWorldPoint(b2Vec2(0.f, -0.5f));
                stunnedTarget->getBody()->SetTransform(offset, getBody()->GetAngle());

            }

            break;
        }
    }
}

bool Trap::checkIfTouching(Entity *entity) {
    return std::find(currentlyTouchingWolves.begin(), currentlyTouchingWolves.end(),
                     entity) != currentlyTouchingWolves.end();
}

bool Trap::checkIfTargetIsDead() {
    return std::find(configGame->entities->begin(), configGame->entities->end(), stunnedTarget) ==
           configGame->entities->end();
}


