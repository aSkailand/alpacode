
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
    setID(Entity::ID::PLANET);
    body->SetUserData((void *) this);

    // Create SFML Shape
    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setFillColor(sf::Color::White);
    sfShape->setTexture(&configGame->planetTexture);
    sfShape->setOutlineThickness(3);
    sfShape->setOutlineColor(sf::Color::Black);

}

void Planet::render(sf::RenderWindow *window) {

    float shape_x = getBody()->GetPosition().x * SCALE;
    float shape_y = getBody()->GetPosition().y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    window->draw(*sfShape);
}

void Planet::startContact(Entity *contactEntity) {
}

void Planet::endContact(Entity *contactEntity) {
}

bool Planet::deadCheck() {
    return false;
}

