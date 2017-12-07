
#include "planet.h"

Planet::Planet(b2World *world, ConfigGame *configGame, float radius, float x, float y) {

    this->configGame = configGame;

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_staticBody;

    body = world->CreateBody(&bodyDef);

    // Create b2Shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = (configGame->planetRadius / SCALE);

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
    sfShape = new sf::CircleShape(radius + 70);
    sfShape->setOrigin(radius + 70, radius + 70);
    sfShape->setTexture(&configGame->planetTextures[0]);
    sfShape->setOutlineThickness(3);
    sfShape->setOutlineColor(sf::Color::Black);

}

void Planet::render(sf::RenderWindow *window) {

    float shape_x = getBody()->GetPosition().x * SCALE;
    float shape_y = getBody()->GetPosition().y * SCALE;

    sfShape->setPosition(shape_x, shape_y);
    window->draw(*sfShape);

    for (int i = 0; i < 6; ++i) {
        sf::Vertex line[2] =
                {
                        sf::Vertex(sf::Vector2f(configGame->planetCenter), sf::Color::Black),
                        sf::Vertex(
                                sf::Vector2f(configGame->calcX(60.f * i, 1500.f), configGame->calcY(60.f * i, 1500.f)))
                };
        window->draw(line, 2, sf::Lines);
    }


}

void Planet::startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {

}

void Planet::endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) {

}

bool Planet::deadCheck() {
    return false;
}

void Planet::setTexture(sf::Texture *texture) {
    sfShape->setTexture(texture);
}

