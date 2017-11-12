
#include "Shotgun.h"
#include "Bullet.h"


Shotgun::Shotgun(b2World *world, ConfigGame *configGame, float length, float height, float x, float y) {

    this->configGame = configGame;
    this->world = world;
    this->length = length;

    // Create Body
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;
    body = world->CreateBody(&bodyDef);

    // Create Body
    b2PolygonShape b2shape;
    b2shape.SetAsBox(length / 2 / SCALE, height / 2 / SCALE);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0;
    fixtureDef.shape = &b2shape;
    fixtureDef.filter.categoryBits = (uint16) ID::SHOTGUN;
    fixtureDef.filter.maskBits = (uint16) ID::PLANET | (uint16) ID::WOLF | (uint16) ID::ALPACA;

    // Create Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = length / 2 / SCALE;
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
    body->SetUserData((void *) this);

    // Create SFML shape
    sfShape = new sf::RectangleShape(sf::Vector2f(length, height));
    sfShape->setOrigin(length / 2, height / 2);
//    sfShape->setFillColor(sf::Color::Magenta);
    sfShape->setOutlineThickness(1);
    sfShape->setOutlineColor(sf::Color::Black);

    sf::Texture *texture = new sf::Texture;
    texture->loadFromFile("entity/shotgun/shotgun.png");
    sfShape->setTexture(texture);


}

void Shotgun::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;

    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);

    // todo: delete
    b2Vec2 muzzle = getBody()->GetWorldPoint(b2Vec2(length / 2 / SCALE, 0));
    b2Vec2 toTarget = muzzle - b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
    b2Vec2 tempy = toTarget;
    tempy.Normalize();
    auto *shape = new sf::CircleShape(3);
    shape->setPosition(toTarget.x * SCALE, toTarget.y * SCALE);

    auto shape2 = new sf::CircleShape(3);
    shape2->setFillColor(sf::Color::Magenta);
    shape2->setPosition(muzzle.x * SCALE, muzzle.y * SCALE);

    window->draw(*shape);
    window->draw(*shape2);

}

void Shotgun::startContact(Entity *contactEntity) {

}

void Shotgun::endContact(Entity *contactEntity) {

}

void Shotgun::use() {

    shootBullets(40.f, 10.f, 5);



}

void Shotgun::shootBullets(float bulletForce, float coneAngle, int numBullets) {

    b2Vec2 muzzle = getBody()->GetWorldPoint(b2Vec2(length / 2 / SCALE, 0));
//
//    float force = 30.f;
//    float angle = 30.f; // Degree
//    int numBullets = 100;

    // FUNCTION HERE:
//    float angle = coneAngle / DEGtoRAD;
    float upperAngle = coneAngle / 2.0f / DEGtoRAD;
    float deltaAngle = coneAngle / (float) numBullets / DEGtoRAD;

    b2Vec2 toTarget = muzzle - b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE);
    toTarget.Normalize();

    float toTargetAngle = getBody()->GetAngle() + upperAngle;

    for (int i = 0; i < numBullets; ++i) {

        auto *tempBullet = new Bullet(world, configGame, 5, muzzle);
        float mass = tempBullet->getBody()->GetMass();

        b2Vec2 offset = b2Vec2(-cos(toTargetAngle - (i * deltaAngle)), -sin(toTargetAngle - (i * deltaAngle)));
        b2Vec2 localOffset = tempBullet->getBody()->GetWorldVector(offset);
        localOffset.Normalize();

        tempBullet->getBody()->ApplyLinearImpulseToCenter(bulletForce * mass * -localOffset, true);

        configGame->entities->emplace_back(tempBullet);
    }
}

bool Shotgun::deadCheck() {
    return false;
}
