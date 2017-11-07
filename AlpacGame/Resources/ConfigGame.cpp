
#include "ConfigGame.h"
#include "../entity/CollisionListener.h"
#include "../entity/planet/planet.h"
#include "../entity/player/farmer.h"

void ConfigGame::run(sf::RenderWindow &window) {

    this->windowPointer = &window;
    windowWidth = window.getSize().x;
    windowHeigth = window.getSize().y;

    planetCenter = sf::Vector2f(window.getSize().x / 2, window.getSize().y);


    loadAllTextures();
    loadAllFonts();
}

float ConfigGame::calcX(float angle) {
    float x = planetCenter.x + planetRadius * (float) std::sin(angle * (M_PI / 180));
    return x;
}

float ConfigGame::calcY(float degree) {
    float y = planetCenter.y - planetRadius * (float) std::cos(degree * (M_PI / 180));
    return y;
}

float ConfigGame::calcX(float degree, float radius) {
    float x = planetCenter.x + radius * (float) std::sin(degree * (M_PI / 180));
    return x;
}

float ConfigGame::calcY(float degree, float radius) {
    float y = planetCenter.y - radius * (float) std::cos(degree * (M_PI / 180));
    return y;
}

void ConfigGame::loadAllFonts() {
    fontID.loadFromFile("Resources/fontPixel.ttf");
}

void ConfigGame::loadAllTextures() {
    planetTexture.loadFromFile("entity/planet/planet.png");
    farmerTexture.loadFromFile("entity/player/farmer.png");
    alpacaTexture.loadFromFile("entity/alpaca/alpaca.png");
    wolfTexture.loadFromFile("entity/wolf/wolfy.png");
}

void ConfigGame::reset() {

    /// Initiating World (With no innate gravitation)
    delete world;
    world = new b2World(b2Vec2(0, 0));
    world->SetContactListener(new CollisionListener());

    /// Instantiating initial entities
    delete planet;
    planet = new Planet(world, this, planetRadius, planetCenter.x, planetCenter.y);
    planetBody = planet->getBody();

    delete farmer;
    farmer = new Farmer(world, this, 50, 0, -200);

    delete entities;
    entities = new std::vector<Entity*>;
    entities->push_back(farmer);
    entities->push_back(planet);

    Wolf::nextId = 0;
    Alpaca::nextId = 0;

}
