
#include "planet.h"

Planet::Planet(ConfigGame *configGame, float radius, float x, float y) {

    this->configGame = configGame;

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_staticBody;

    body = configGame->world->CreateBody(&bodyDef);

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
    setEntity_ID(Entity::ID::PLANET);
    body->SetUserData((void *) this);

    // Create SFML Shape
    int offset = 80;
    sf_ShapeEntity = new sf::CircleShape(radius + offset);
    sf_ShapeEntity->setOrigin(radius + offset, radius + offset);
    sf_ShapeEntity->setTexture(&this->configGame->planetTextures[0]);
    sf_ShapeEntity->setOutlineColor(sf::Color::Black);
    sf_ShapeEntity->setOutlineThickness(0);

    sf_DebugBody = new sf::CircleShape(radius);
    sf_DebugBody->setOrigin(radius, radius);
    sf_DebugBody->setFillColor(sf::Color::Transparent);
    sf_DebugBody->setOutlineColor(sf::Color::Black);
    sf_DebugBody->setOutlineThickness(2);

    shapeBackground = sf::CircleShape(radius + offset);
    shapeBackground.setOrigin(radius + offset, radius + offset);
    shapeBackground.setTexture(&this->configGame->planetTextures[1]);

    // Set fixed position
    float shape_x = getBody()->GetPosition().x * SCALE;
    float shape_y = getBody()->GetPosition().y * SCALE;

    sf_ShapeEntity->setPosition(shape_x, shape_y);
    shapeBackground.setPosition(shape_x, shape_y);
    sf_DebugBody->setPosition(shape_x, shape_y);

}

void Planet::render(sf::RenderWindow *window) {

    // Change opacity by delta
    if(!configGame->isPaused){
        sf_ShapeEntity->setFillColor(sf_ShapeEntity->getFillColor() - sf::Color(0,0,0,2));
        shapeBackground.setFillColor(shapeBackground.getFillColor() + sf::Color(0,0,0,2));
    }

    // Draw Planet
    window->draw(shapeBackground);
    window->draw(*sf_ShapeEntity);

    // Draw if debug mode is on
    if (configGame->showDebugMode) {

        // Draw debug vertices to split planet in six slices
        for (int i = 0; i < 6; ++i) {
            sf::Vertex line[2];
            line[0] = sf::Vertex(sf::Vector2f(configGame->planetCenter), sf::Color::Black);
            line[1] = sf::Vertex(sf::Vector2f(configGame->calcX(60.f * i, 1500.f), configGame->calcY(60.f * i, 1500.f)), sf::Color::Black);

            window->draw(line, 2 , sf::Lines);
        }

        // Debug draw
        window->draw(*sf_DebugBody);

    }


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

void Planet::setTexture(unsigned int cycleFrame) {

    // Change opacity of foreground/background
    sf_ShapeEntity->setFillColor(sf_ShapeEntity->getFillColor() + sf::Color(0,0,0,255));
    shapeBackground.setFillColor(shapeBackground.getFillColor() - sf::Color(0,0,0,150));

    // Switch textures accordingly to cycle frame
    sf_ShapeEntity->setTexture(&configGame->planetTextures[cycleFrame]);
    shapeBackground.setTexture(&configGame->planetTextures[(cycleFrame == 11) ? 0 : ++cycleFrame]);

}

