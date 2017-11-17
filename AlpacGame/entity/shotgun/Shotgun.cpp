
#include "Shotgun.h"
#include "Bullet.h"

Shotgun::Shotgun(ConfigGame *configGame, float length, float height, float x, float y) {

    // Assign pointers
    this->configGame = configGame;
    this->world = configGame->world;
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
    sfShape->setOutlineColor(sf::Color::Black);
//    sfShape->setTexture(&configGame->shotgunHeldTexture);

    sf_HitSensor = new sf::CircleShape(length / 2);
    sf_HitSensor->setFillColor(sf::Color::Transparent);
    sf_HitSensor->setOrigin(length / 2, length / 2);

}

void Shotgun::render(sf::RenderWindow *window) {

    // Render sfShape
    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    if(isHeld){
        sfShape->setScale(1.f, configGame->mouseInLeftSide ? -1.f : 1.f);
        sfShape->setTexture(&configGame->shotgunHeldTexture);
    }
    else{
        sfShape->setScale(1.f, 1.f);
        sfShape->setTexture(&configGame->shotgunDropTexture);
    }

    window->draw(*sfShape);

    if(configGame->showLabels){
        // Draw sfShape Debug
        sfShape->setOutlineThickness(2);

        // Draw hitSensor debug
        sf_HitSensor->setOutlineThickness(2);
        sf_HitSensor->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_HitSensor->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_HitSensor);
    }
    else{
        sfShape->setOutlineThickness(0);
    }
}

void Shotgun::startContact(Entity *contactEntity) {

}

void Shotgun::endContact(Entity *contactEntity) {

}

void Shotgun::use() {

    // Shoot bullets
    shootBullets(40.f, 10.f, 5);

    // Recoil push
    b2Body *farmerBody = configGame->farmer->getBody();
    float mass = farmerBody->GetMass();
    b2Vec2 muzzle = getBody()->GetWorldPoint(b2Vec2(length / 2 / SCALE, 0));
    b2Vec2 toTarget = b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE) - muzzle;
    toTarget.Normalize();
    farmerBody->ApplyLinearImpulseToCenter(10.f * mass * -toTarget, true);

}

void Shotgun::shootBullets(float bulletForce, float coneAngle, int numBullets) {

    b2Vec2 muzzle = getBody()->GetWorldPoint(b2Vec2(length / 2 / SCALE, 0));

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
