
#include "planet.h"

Planet::Planet(b2World *world, ConfigGame *configGame, float radius, float x, float y) {

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_staticBody;

    body = world->CreateBody(&bodyDef);

    // Create b2Shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = (radius / SCALE);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.shape = &b2Shape;
    fixtureDef.filter.categoryBits = categoryBits;
    fixtureDef.filter.maskBits = maskBits;

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    // Store info
    setEntity_ID(Entity::ID::PLANET);
    body->SetUserData((void *) this);

    // Create SFML Shape
    sf_ShapeEntity = new sf::CircleShape(radius);
    sf_ShapeEntity->setOrigin(radius, radius);
    sf_ShapeEntity->setFillColor(sf::Color::White);
    sf_ShapeEntity->setTexture(&configGame->planetTexture);
    sf_ShapeEntity->setOutlineThickness(3);
    sf_ShapeEntity->setOutlineColor(sf::Color::Black);

}

void Planet::render(sf::RenderWindow *window) {

    float shape_x = getBody()->GetPosition().x * SCALE;
    float shape_y = getBody()->GetPosition().y * SCALE;

    sf_ShapeEntity->setPosition(shape_x, shape_y);
    window->draw(*sf_ShapeEntity);
}

void Planet::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {

}

void Planet::endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {

}

bool Planet::deadCheck() {
    return false;
}

void Planet::update() {
}

void Planet::pause() {
}

void Planet::resume() {
}

