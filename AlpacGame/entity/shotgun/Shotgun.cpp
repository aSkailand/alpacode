
#include "Shotgun.h"
#include "Bullet.h"

Shotgun::Shotgun(ConfigGame *configGame, ConfigSound *configSound, float length, float height, float x, float y) {

    // Assign pointers
    this->configGame = configGame;
    this->configSound = configSound;
    this->world = configGame->world;
    this->length = length;
    this->bulletIndicatorTextures = configGame->bulletIndicatorTextures;

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
    fixtureDef.restitution = 0.5f;
    fixtureDef.shape = &b2shape;
    fixtureDef.filter.categoryBits = (uint16) ID::SHOTGUN;
    fixtureDef.filter.maskBits = (uint16) ID::PLANET;

    // Create Sensor
    b2CircleShape b2Shape2;
    b2Shape2.m_radius = length / 2 / SCALE;
    b2FixtureDef sensor;
    sensor.shape = &b2Shape2;
    sensor.isSensor = true;
    sensor.filter.categoryBits = (uint16) ID::SHOTGUN;
    sensor.filter.maskBits = (uint16) ID::FARMER;

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef);
    fixture_hit = body->CreateFixture(&sensor);

    // Set Fixture Sense to given Enums
    fixture_body->SetUserData(convertToVoidPtr((int) CollisionID::BODY));
    fixture_hit->SetUserData(convertToVoidPtr((int) CollisionID::HIT));

    // Store Information
    setEntity_ID(Entity::ID::SHOTGUN);
    body->SetUserData((void *) this);

    // Create SFML shape
    sf_ShapeEntity = new sf::RectangleShape(sf::Vector2f(length, height));
    sf_ShapeEntity->setOrigin(length / 2, height / 2);
    sf_ShapeEntity->setOutlineColor(sf::Color::Black);

    sf_DebugHit = new sf::CircleShape(length / 2);
    sf_DebugHit->setFillColor(sf::Color::Transparent);
    sf_DebugHit->setOrigin(length / 2, length / 2);

    // Assigning bullets
    numCurrentBullets = numMaxBullets;

    // Readying clock
    reloadClock.reset(false);
    reloadCooldownBarometer = new CooldownBarometer(configGame->cooldownTextures, &reloadClock, 35.f, 35.f);

    indicatorShape.setSize(sf::Vector2f(35.f, 35.f));
    indicatorShape.setOrigin(0.5f * indicatorShape.getSize());
    indicatorShape.setTexture(&bulletIndicatorTextures[0]);

}

void Shotgun::render(sf::RenderWindow *window) {

    // Render sf_ShapeEntity
    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sf_ShapeEntity->setPosition(shape_x, shape_y);
    sf_ShapeEntity->setRotation((body->GetAngle() * DEGtoRAD));

    if (!configGame->isPaused) {
        if (isHeld) {
            sf_ShapeEntity->setScale(1.f, configGame->mouseInLeftSide ? -1.f : 1.f);
            sf_ShapeEntity->setTexture(&configGame->shotgunHeldTexture);
        } else {
            sf_ShapeEntity->setScale(1.f, 1.f);
            sf_ShapeEntity->setTexture(&configGame->shotgunDropTexture);
        }
    }

    window->draw(*sf_ShapeEntity);

    if (isHeld) {

        // Reload Cooldown Barometer
        reloadCooldownBarometer->setPlacement(
                configGame->farmer->getBody()->GetWorldPoint(b2Vec2(-0.5f, -4.2f)).x * SCALE,
                configGame->farmer->getBody()->GetWorldPoint(b2Vec2(-0.5f, -4.2f)).y * SCALE,
                configGame->farmer->getSf_ShapeEntity()->getRotation());
        reloadCooldownBarometer->render(window);

        // Draw bullet indicator shape
        indicatorShape.setPosition(configGame->farmer->getBody()->GetWorldPoint(b2Vec2(-2.0f, -4.2f)).x * SCALE,
                                   configGame->farmer->getBody()->GetWorldPoint(b2Vec2(-2.0f, -4.2f)).y * SCALE);
        indicatorShape.setRotation(configGame->farmer->getSf_ShapeEntity()->getRotation());
        indicatorShape.setTexture(&bulletIndicatorTextures[2-numCurrentBullets]);
        window->draw(indicatorShape);
    }

    if (configGame->showDebugMode) {

        // Draw sf_ShapeEntity Debug
        sf_ShapeEntity->setOutlineThickness(2);

        // Draw hitSensor debug
        sf_DebugHit->setOutlineThickness(2);
        sf_DebugHit->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
        sf_DebugHit->setRotation(body->GetAngle() * DEGtoRAD);
        window->draw(*sf_DebugHit);

        if (farmerTouch) {
            sf_ShapeEntity->setOutlineColor(sf::Color::Yellow);
        } else {
            sf_ShapeEntity->setOutlineColor(sf::Color::Black);
        }
    } else {
        sf_ShapeEntity->setOutlineThickness(0);
    }
}

void Shotgun::use() {

    if (numCurrentBullets > 0) {

        // Shoot bullets
        shootBullets(20.f, 10.f, 5);

        // Plays a shotgun sound
        configSound->mapSounds[ConfigSound::soundsID::SHOTGUN].play();

        // Decrement number of bullets
        numCurrentBullets--;

        // Recoil push
        b2Body *farmerBody = configGame->farmer->getBody();
        float mass = farmerBody->GetMass();
        b2Vec2 muzzle = getBody()->GetWorldPoint(b2Vec2(length / 2 / SCALE, 0));
        b2Vec2 toTarget = b2Vec2(configGame->mouseXpos / SCALE, configGame->mouseYpos / SCALE) - muzzle;
        toTarget.Normalize();
        farmerBody->ApplyLinearImpulseToCenter(10.f * mass * -toTarget, true);

    } else {
        // todo: Add a sound for empty barrels
    }

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

void
Shotgun::startContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

}

void Shotgun::endContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

}

void Shotgun::performHold() {
    isHeld = true;
}

void Shotgun::performThrow() {
    isHeld = false;
}

void Shotgun::update() {

    if (!reloadClock.isRunning()) {
        if (isHeld && numCurrentBullets < numMaxBullets) {
            reloadClock.reset(true);
            reloadCooldownBarometer->calcTicks(reloadTick);
        }
    } else {
        if(!isHeld){
            reloadClock.reset(false);
        }
        else if(reloadClock.getElapsedTime().asSeconds() > reloadTick){
            numCurrentBullets++;
            reloadClock.reset(false);
        }
    }

}


void Shotgun::pause() {
}

void Shotgun::resume() {

}
