
#include <cmath>
#include <iostream>
#include "ConfigGame.h"
#include "../entity/CollisionListener.h"
#include "../entity/planet/planet.h"

void ConfigGame::run(sf::RenderWindow &window) {

    this->window = &window;

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

void ConfigGame::loadTexture(std::string filename,
                             std::map<EntityWarm::Action, SpriteInfo> &spriteMap,
                             int width,
                             int height,
                             int top,
                             EntityWarm::Action action) {

    for (int x = spriteMap[action].startFrame; x <= spriteMap[action].endFrame; ++x) {

        auto *texture = new sf::Texture();

        if (!texture->loadFromFile(filename, sf::IntRect(x * width, top, width, height))) {
            std::cout << "Error loading file!" << std::endl;
        }

        spriteMap[action].sprites.push_back(texture);
    }
}

void ConfigGame::loadAllTextures() {

    /// Mapping sprites for entities

    // Planet
    planetTexture.loadFromFile("entity/planet/planet.png");

    morning_1.loadFromFile("entity/planet/1_Morning.png");
    morning_2.loadFromFile("entity/planet/2_Morning.png");
    morning_3.loadFromFile("entity/planet/3_Morning.png");
    afternoon_4.loadFromFile("entity/planet/4_Afternoon.png");
    afternoon_5.loadFromFile("entity/planet/5_Afternoon.png");
    afternoon_6.loadFromFile("entity/planet/6_Afternoon.png");
    evening_7.loadFromFile("entity/planet/7_Evening.png");
    evening_8.loadFromFile("entity/planet/8_Evening.png");
    evening_9.loadFromFile("entity/planet/9_Evening.png");
    night_10.loadFromFile("entity/planet/10_Night.png");
    night_11.loadFromFile("entity/planet/11_Night.png");
    night_12.loadFromFile("entity/planet/12_Night.png");

    // Shotgun
    shotgunHeldTexture.loadFromFile("entity/shotgun/shotgunHeld.png");
    shotgunDropTexture.loadFromFile("entity/shotgun/shotgunDrop.png");

    // Farmer
    std::string farmerSpriteSheet = "entity/player/farmer-sprite.png";

    int farmerWidth = 50;
    int farmerHeight = 70;

    farmerSprites[EntityWarm::Action::IDLE].startFrame = 0;
    farmerSprites[EntityWarm::Action::IDLE].endFrame = 2;
    loadTexture(farmerSpriteSheet, farmerSprites, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::IDLE);

    farmerSprites[EntityWarm::Action::WALKING].startFrame = 3;
    farmerSprites[EntityWarm::Action::WALKING].endFrame = 8;
    loadTexture(farmerSpriteSheet, farmerSprites, farmerWidth, farmerHeight, 0 ,
                EntityWarm::Action::WALKING);

    // Alpaca
    std::string alpacaSpriteSheet = "entity/alpaca/alpaca-sprite.png";

    int alpacaWidth = 100;
    int alpacaHeight = 110;

    alpacaSprites[EntityWarm::Action::IDLE].startFrame = 0;
    alpacaSprites[EntityWarm::Action::IDLE].endFrame = 4;
    loadTexture(alpacaSpriteSheet, alpacaSprites, alpacaWidth, alpacaHeight, 0,
                EntityWarm::Action::IDLE);

    alpacaSprites[EntityWarm::Action::WALKING].startFrame = 0;
    alpacaSprites[EntityWarm::Action::WALKING].endFrame = 1;
    loadTexture(alpacaSpriteSheet, alpacaSprites, alpacaWidth, alpacaHeight, 0,
                EntityWarm::Action::WALKING);


    // Wolf
    std::string wolfSpriteSheet1 = "entity/wolf/wolf-sprite.png";

    int wolfWidth = 145;
    int wolfHeight = 80;

    wolfSprites[EntityWarm::Action::IDLE].startFrame = 0;
    wolfSprites[EntityWarm::Action::IDLE].endFrame = 3;
    loadTexture(wolfSpriteSheet1,wolfSprites,wolfWidth,wolfHeight, 0,
                EntityWarm::Action::IDLE);

    wolfSprites[EntityWarm::Action::WALKING].startFrame = 1;
    wolfSprites[EntityWarm::Action::WALKING].endFrame = 2;
    loadTexture(wolfSpriteSheet1, wolfSprites, wolfWidth, wolfHeight, 0,
                EntityWarm::Action::WALKING);

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

    farmer = nullptr;
    delete farmer;
    farmer = new Farmer(this, 30, 100, 100, 100, -200);

    delete entities;
    entities = new std::vector<Entity *>;
    entities->push_back(farmer);
    entities->push_back(planet);

    Wolf::nextId = 0;
    Alpaca::nextId = 0;

}
