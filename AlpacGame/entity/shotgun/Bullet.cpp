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
    setID(Entity::ID::BULLET);
    body->SetUserData((void *) this);

    // Create SFML Shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setFillColor(sf::Color::White);
    sfShape->setOutlineThickness(2);
    sfShape->setOutlineColor(sf::Color::Black);

    decayClock.restart();

}

void Bullet::render(sf::RenderWindow *window) {

    float shape_x = SCALE * body->GetPosition().x;
    float shape_y = SCALE * body->GetPosition().y;

    sfShape->setPosition(shape_x, shape_y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);
}

bool Bullet::deadCheck() {
    return decayClock.getElapsedTime().asSeconds() > decayTick;
}

Bullet::~Bullet() {

}

void
Bullet::startContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {
    switch (contactEntity->getID()) {
        case ID::PLANET:
            break;
        case ID::FARMER:
            break;
        case ID::ALPACA: {
            dynamic_cast<EntityWarm*> (contactEntity)->dealDamage(1);
            b2Vec2 force = contactEntity->getBody()->GetWorldVector(b2Vec2(0,-1.f));
            float32 mass =contactEntity->getBody()->GetMass();
            contactEntity->getBody()->ApplyLinearImpulseToCenter(mass * force ,true);
            break;
        }
        case ID::WOLF: {
            dynamic_cast<EntityWarm*> (contactEntity)->dealDamage(1);
            break;
        }
        case ID::SHOTGUN:
            break;
        case ID::BULLET:
            break;
    }

    sfShape->setFillColor(sf::Color::Red);
    sfShape->setOutlineThickness(0);

    if(!hit){
        fixture_body->SetFilterData(filter);
        hit = true;
    }
}

void Bullet::endContact(Entity::CollisionID selfCollision, Entity::CollisionID otherCollision, Entity *contactEntity) {

}