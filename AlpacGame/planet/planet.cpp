
#include "planet.h"

Planet::Planet(b2World *world, float radius, float x, float y) {

    loadTextures();

    this->x = x;
    this->y = y;

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->x / SCALE, this->y / SCALE);
    bodyDef.type = b2_staticBody;

    body = world->CreateBody(&bodyDef);

    // Create b2Shape
    b2CircleShape b2Shape;
    b2Shape.m_radius = (radius / SCALE);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.friction = 10.0f;

    // Connect b2Shape to fixture
    fixtureDef.shape = &b2Shape;
    fixtureDef.filter.categoryBits = (uint16) ID::PLANET;
    fixtureDef.filter.maskBits = (uint16) ID::FARMER | (uint16) ID::ALPACA | (uint16) ID::WOLF;

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    setID(Entity::ID::PLANET);
    body->SetUserData((void *) this);

    sfShape = new sf::CircleShape(radius);
    sfShape->setOrigin(radius, radius);
    sfShape->setFillColor(sf::Color::White);
    sfShape->setTexture(&planetTexture);
    sfShape->setOutlineThickness(3);
    sfShape->setOutlineColor(sf::Color::Black);
}

//Planet::Planet(StateMachine &stateMachine) {
//
//    // Assign pointers
//    machine = &stateMachine;
//    window = &machine->configWindow.getWindow();
//
//    // Load textures
//    loadTextures();
//
//    // Define Background
//    backgroundSprite = sf::Sprite(backgroundTexture);
//
//    // Creating Planet
//    float radius = stateMachine.configGame.planetRadius;
//    planet = sf::CircleShape(radius);
//    planet.setTexture(&planetTexture);
//    planet.setOrigin(sf::Vector2f(radius, radius));
//    planet.setPosition(stateMachine.configGame.planetCenter);
//
//}

//void Planet::draw() {
//    window->draw(backgroundSprite);
//    window->draw(planet);
//}

void Planet::loadTextures() {

    if (!planetTexture.loadFromFile("planet/planet.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("planet/back.png")) {
        std::cout << "Error!!!" << std::endl;
    }
}

void Planet::adjust() {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
}

void Planet::switchAction() {
    return;
}
