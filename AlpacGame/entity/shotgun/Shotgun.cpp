
#include "Shotgun.h"


Shotgun::Shotgun(b2World *world, ConfigGame *configGame, float width, float height, float x, float y) {

    this->configGame = configGame;

    // Create Body
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->x / SCALE, this->y / SCALE);
    bodyDef.type = b2_dynamicBody;
    body = world->CreateBody(&bodyDef);

    // Create Body
    b2PolygonShape b2shape;
    b2shape.SetAsBox(width / 2 / SCALE, height / 2 / SCALE);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.1f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0;
    fixtureDef.shape = &b2shape;
    fixtureDef.filter.categoryBits = (uint16) ID::SHOTGUN;
    fixtureDef.filter.maskBits = (uint16) ID::PLANET;

    // Create Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = width / 2 / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::SHOTGUN;
    sensor.filter.maskBits = (uint16) ID::FARMER;

    // Connect fixture to body
    bodyFixture = body->CreateFixture(&fixtureDef);
    sensorFixture = body->CreateFixture(&sensor);

    // Store Information
    setID(Entity::ID::SHOTGUN);
    body->SetUserData((void*) this);

    // Create SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
    sfShape->setOrigin(width/2,height/2);
    sfShape->setFillColor(sf::Color::Magenta);
    sfShape->setOutlineThickness(3);
    sfShape->setOutlineColor(sf::Color::Black);

}

void Shotgun::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;

    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);
}

void Shotgun::startContact(Entity *contactEntity) {

}

void Shotgun::endContact(Entity *contactEntity) {

}
