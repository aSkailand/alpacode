//
// Created by Trong on 09/11/2017.
//

#include "Bullet.h"
#include "../EntityWarm.h"

Bullet::Bullet(b2World *world, ConfigGame *configGame, float radius, b2Vec2 position) {

    this->configGame = configGame;

    filter.categoryBits = (uint16) ID::BULLET;
    filter.maskBits = (uint16) ID::PLANET;

    // Create Body
    b2BodyDef bodyDef;
    bodyDef.position = position;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    body = world->CreateBody(&bodyDef);

    // Create Fixture
    b2CircleShape b2Shape;
    b2Shape.m_radius = (radius / SCALE);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = (uint16) ID::BULLET;

    fixtureDef.filter.maskBits = (uint16) ID::PLANET
                                 | (uint16) ID::ALPACA
                                 | (uint16) ID::WOLF;

    fixtureDef.shape = &b2Shape;

    // Connect fixture to body
    fixture_body = body->CreateFixture(&fixtureDef);

    // Store info
    setEntity_ID(Entity::ID::BULLET);
    body->SetUserData((void *) this);

    // Create SFML Shape
    sf_ShapeEntity = new sf::CircleShape(radius);
    sf_ShapeEntity->setOrigin(radius, radius);
    sf_ShapeEntity->setFillColor(sf::Color::White);
    sf_ShapeEntity->setOutlineThickness(2);
    sf_ShapeEntity->setOutlineColor(sf::Color::Black);

    decayClock.restart();

}

void Bullet::render(sf::RenderWindow *window) {

    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sf_ShapeEntity->setPosition(shape_x, shape_y);
    sf_ShapeEntity->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sf_ShapeEntity);
}

bool Bullet::deadCheck() {
    return decayClock.getElapsedTime().asSeconds() > decayTick;
}

Bullet::~Bullet() {

}

void
Bullet::startContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getEntity_ID()) {
        case ID::PLANET:
            break;
        case ID::FARMER:
            break;
        case ID::ALPACA: {
            dynamic_cast<EntityWarm *> (contactEntity)->dealDamage(1);
            b2Vec2 force = contactEntity->getBody()->GetWorldVector(b2Vec2(0, -2.f));
            float32 mass = contactEntity->getBody()->GetMass();
            contactEntity->getBody()->ApplyLinearImpulseToCenter(mass * force, true);
            break;
        }
        case ID::WOLF: {
            dynamic_cast<EntityWarm *> (contactEntity)->dealDamage(1);
            b2Vec2 force = contactEntity->getBody()->GetWorldVector(b2Vec2(0, -2.f));
            float32 mass = contactEntity->getBody()->GetMass();
            contactEntity->getBody()->ApplyLinearImpulseToCenter(mass * force, true);
            break;
        }
        default:
            break;
    }

    sf_ShapeEntity->setFillColor(sf::Color::Red);
    sf_ShapeEntity->setOutlineThickness(0);

    if (!hit) {
        fixture_body->SetFilterData(filter);
        hit = true;
    }
}

void Bullet::endContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

}

void Bullet::update() {

}
