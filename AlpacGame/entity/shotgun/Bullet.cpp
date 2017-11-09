//
// Created by Trong on 09/11/2017.
//

#include "Bullet.h"

Bullet::Bullet(b2World *world, ConfigGame *configGame, float radius, b2Vec2 position) {

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
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = (uint16) ID::BULLET;
    fixtureDef.filter.maskBits =   (uint16) ID::PLANET
                                 | (uint16) ID::ALPACA
                                 | (uint16) ID::WOLF;
    fixtureDef.shape = &b2Shape;

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    // Store info
    setID(Entity::ID::BULLET);
    body->SetUserData((void *) this);

    // Create SFML Shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setFillColor(sf::Color::White);
    sfShape->setOutlineThickness(2);
    sfShape->setOutlineColor(sf::Color::Black);

}

void Bullet::render(sf::RenderWindow *window) {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;

    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));

    window->draw(*sfShape);
}

void Bullet::startContact(Entity *contactEntity) {
    switch (contactEntity->getID()){
        case ID::PLANET:break;
        case ID::FARMER:break;
        case ID::ALPACA:{
            sfShape->setFillColor(sf::Color::Red);
            sfShape->setOutlineThickness(0);
            break;
        }
        case ID::WOLF:{
            sfShape->setFillColor(sf::Color::Red);
            sfShape->setOutlineThickness(0);
            break;
        }
        case ID::SHOTGUN:break;
        case ID::BULLET:break;
    }
}

void Bullet::endContact(Entity *contactEntity) {

}
