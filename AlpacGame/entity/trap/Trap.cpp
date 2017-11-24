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
    b2Shape2.m_radius = length / 2 / SCALE;
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
    sfShape->setFillColor(sf::Color::Transparent);
    sfShape->setOutlineThickness(3);
    sfShape->setOutlineColor(sf::Color::Black);

    sf_HitSensor = new sf::CircleShape(length / 2);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(length / 2, length / 2);

}

void Trap::render(sf::RenderWindow *window) {

    // Render sfShape
    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    if(activated){
        sfShape->setOutlineColor(sf::Color::Red);
    }
    else{
        sfShape->setOutlineColor(sf::Color::Black);
    }

//    if (isHeld) {
//        sfShape->setScale(1.f, configGame->mouseInLeftSide ? -1.f : 1.f);
//        sfShape->setTexture(&configGame->shotgunHeldTexture);
//    } else {
//        sfShape->setScale(1.f, 1.f);
//        sfShape->setTexture(&configGame->shotgunDropTexture);
//    }

    window->draw(*sfShape);

    if (configGame->showLabels) {


        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);

        if (farmerTouch) {
            sfShape->setOutlineColor(sf::Color::Yellow);
        } else {
            sfShape->setOutlineColor(sf::Color::Black);
        }
    } else {
    }

}

void Trap::startContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

}

void Trap::endContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

}

bool Trap::deadCheck() {
    return false;
}

void Trap::use() {
    activated = !activated;
}
