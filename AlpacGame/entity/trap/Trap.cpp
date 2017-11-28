//
// Created by Trong on 24/11/2017.
//

#include "Trap.h"
#include "../../Resources/ConfigGame.h"


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
    setID(Entity::ID::TRAP);
    body->SetUserData((void *) this);

    // Create SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(length, height));
    sfShape->setOrigin(length / 2, height / 2);

    sf_HitSensor = new sf::CircleShape(fixture_hit->GetShape()->m_radius * SCALE);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(fixture_hit->GetShape()->m_radius * SCALE, fixture_hit->GetShape()->m_radius * SCALE);

}

void Trap::render(sf::RenderWindow *window) {

    switch (currentMode) {
        case Mode::CLOSED:{
            sf_HitSensor->setOutlineColor(sf::Color::Black);
            sfShape->setTexture(&configGame->trapClosedTexture);
            break;
        }
        case Mode::OPEN:{
            sf_HitSensor->setOutlineColor(sf::Color::White);
            sfShape->setTexture(&configGame->trapOpenTexture);
            break;
        }
        case Mode::READY:{
            sf_HitSensor->setOutlineColor(sf::Color::Green);
            sfShape->setTexture(&configGame->trapOpenTexture);
            break;
        }
        case Mode::LATCHED:{
            sf_HitSensor->setOutlineColor(sf::Color::Red);
            sfShape->setTexture(&configGame->trapClosedTexture);
            break;

        }
    }

    /// Render sfShape
    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));


    if (configGame->showLabels) {

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(shape_x, shape_y);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

        sfShape->setOutlineThickness(3);
        sfShape->setOutlineColor(farmerTouch ? sf::Color::Yellow : sf::Color::Black);

    } else{
        sfShape->setOutlineThickness(0);
    }

    window->draw(*sfShape);
}

void Trap::startContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

    switch (contactEntity->getID()) {
        case ID::PLANET: {
            if (selfCollision == Entity::CollisionID::BODY) {
                currentStatus = Status::GROUNDED;
            }
            break;
        }
        case ID::WOLF: {

            if (!checkIfTouching(contactEntity) && otherCollision == Entity::CollisionID::HIT) {
                currentlyTouchingEntities.push_back(dynamic_cast<Wolf *>(contactEntity));
            }
            break;
        }
        default: {
            break;
        }

    }

}

void Trap::endContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET: {
            if (selfCollision == Entity::CollisionID::BODY) {
                currentStatus = Status::AIRBORNE;
            }
            break;
        }
        case ID::WOLF: {
            if (checkIfTouching(contactEntity) && otherCollision == Entity::CollisionID::HIT) {
                currentlyTouchingEntities.remove(dynamic_cast<Wolf *>(contactEntity));
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

void Trap::performStun() {

}

void Trap::performHold() {

    currentStatus = Status::AIRBORNE;

    if (currentMode == Mode::CLOSED) {
        trapClock.reset(true);
        printf("Opening...\n");
    }
    else if (currentMode == Mode::READY) {
        currentMode = Mode::OPEN;
    }
}

void Trap::performThrow() {
    if (currentMode == Mode::CLOSED) {
        trapClock.reset(false);
        printf("Closed...\n");
    }
    else if(currentMode == Mode::OPEN){
        currentMode = Mode::READY;
    }
}

void Trap::update() {

    switch (currentMode) {
        case Mode::CLOSED: {

            if (trapClock.getElapsedTime().asSeconds() >= openTick) {
                currentMode = Mode::OPEN;
                trapClock.reset(false);

                printf("Open!\n");
            }
            break;
        }
        case Mode::OPEN: {
            break;
        }
        case Mode::READY: {
            if (!currentlyTouchingEntities.empty()) {
                stunnedTarget = currentlyTouchingEntities.front();
                stunnedTarget->performStun(this);
                currentMode = Mode::LATCHED;
                trapClock.reset(true);
            }
            break;
        }
        case Mode::LATCHED: {

            if (!stunnedTarget->isStunned || trapClock.getElapsedTime().asSeconds() >= stunTick) {

                printf("Unlatched!\n");

                stunnedTarget->isStunned = false;
                stunnedTarget = nullptr;
                currentMode = Mode::CLOSED;
                trapClock.reset(false);

            } else {
                b2Vec2 offset = getBody()->GetWorldPoint(b2Vec2(0.f, -2.f));
                stunnedTarget->getBody()->SetTransform(offset, stunnedTarget->getBody()->GetAngle());
            }

            break;
        }
    }
}

bool Trap::checkIfTouching(Entity *entity) {
    return std::find(currentlyTouchingEntities.begin(), currentlyTouchingEntities.end(),
                     entity) != currentlyTouchingEntities.end();
}


