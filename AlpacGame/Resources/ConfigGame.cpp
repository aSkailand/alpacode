
#include <cmath>
#include <iostream>
#include "ConfigGame.h"
#include "../entity/CollisionListener.h"
#include "../entity/planet/planet.h"
#include "../scenery/Sun/Sun.h"
#include "../scenery/Sky/Sky.h"

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

    // Sky
    sf::Texture morning_1;
    sf::Texture morning_2;
    sf::Texture morning_3;
    sf::Texture afternoon_4;
    sf::Texture afternoon_5;
    sf::Texture afternoon_6;
    sf::Texture evening_7;
    sf::Texture evening_8;
    sf::Texture evening_9;
    sf::Texture night_10;
    sf::Texture night_11;
    sf::Texture night_12;

    morning_1.loadFromFile(  "scenery/Background/1_Morning.png");
    morning_2.loadFromFile(  "scenery/Background/2_Morning.png");
    morning_3.loadFromFile(  "scenery/Background/3_Morning.png");
    afternoon_4.loadFromFile("scenery/Background/4_Afternoon.png");
    afternoon_5.loadFromFile("scenery/Background/5_Afternoon.png");
    afternoon_6.loadFromFile("scenery/Background/6_Afternoon.png");
    evening_7.loadFromFile(  "scenery/Background/7_Evening.png");
    evening_8.loadFromFile(  "scenery/Background/8_Evening.png");
    evening_9.loadFromFile(  "scenery/Background/9_Evening.png");
    night_10.loadFromFile(   "scenery/Background/10_Night.png");
    night_11.loadFromFile(   "scenery/Background/11_Night.png");
    night_12.loadFromFile(   "scenery/Background/12_Night.png");

    skyTextures.push_back(morning_1);
    skyTextures.push_back(morning_2);
    skyTextures.push_back(morning_3);
    skyTextures.push_back(afternoon_4);
    skyTextures.push_back(afternoon_5);
    skyTextures.push_back(afternoon_6);
    skyTextures.push_back(evening_7);
    skyTextures.push_back(evening_8);
    skyTextures.push_back(evening_9);
    skyTextures.push_back(night_10);
    skyTextures.push_back(night_11);
    skyTextures.push_back(night_12);

    // Planet
    sf::Texture earth_1;
    sf::Texture earth_2;
    sf::Texture earth_3;
    sf::Texture earth_4;
    sf::Texture earth_5;
    sf::Texture earth_6;
    sf::Texture earth_7;
    sf::Texture earth_8;
    sf::Texture earth_9;
    sf::Texture earth_10;
    sf::Texture earth_11;
    sf::Texture earth_12;

    earth_1.loadFromFile( "scenery/DayCycle/Earth/Earth_1.png");
    earth_2.loadFromFile( "scenery/DayCycle/Earth/Earth_2.png");
    earth_3.loadFromFile( "scenery/DayCycle/Earth/Earth_3.png");
    earth_4.loadFromFile( "scenery/DayCycle/Earth/Earth_4.png");
    earth_5.loadFromFile( "scenery/DayCycle/Earth/Earth_5.png");
    earth_6.loadFromFile( "scenery/DayCycle/Earth/Earth_6.png");
    earth_7.loadFromFile( "scenery/DayCycle/Earth/Earth_7.png");
    earth_8.loadFromFile( "scenery/DayCycle/Earth/Earth_8.png");
    earth_9.loadFromFile( "scenery/DayCycle/Earth/Earth_9.png");
    earth_10.loadFromFile("scenery/DayCycle/Earth/Earth_10.png");
    earth_11.loadFromFile("scenery/DayCycle/Earth/Earth_11.png");
    earth_12.loadFromFile("scenery/DayCycle/Earth/Earth_12.png");

    planetTextures.push_back(earth_1);
    planetTextures.push_back(earth_2);
    planetTextures.push_back(earth_3);
    planetTextures.push_back(earth_4);
    planetTextures.push_back(earth_5);
    planetTextures.push_back(earth_6);
    planetTextures.push_back(earth_7);
    planetTextures.push_back(earth_8);
    planetTextures.push_back(earth_9);
    planetTextures.push_back(earth_10);
    planetTextures.push_back(earth_11);
    planetTextures.push_back(earth_12);

    // Sun/Moon
    sf::Texture sun_1;
    sf::Texture sun_2;
    sf::Texture moon_1;

    sun_1.loadFromFile( "scenery/Sun/Sun_1.png");
    sun_2.loadFromFile( "scenery/Sun/Sun_2.png");
    moon_1.loadFromFile("scenery/Sun/Moon_1.png");

    sunTextures.push_back(sun_1);
    sunTextures.push_back(sun_2);
    sunTextures.push_back(moon_1);

    // Shotgun
    shotgunHeldTexture.loadFromFile("entity/shotgun/shotgunHeld.png");
    shotgunDropTexture.loadFromFile("entity/shotgun/shotgunDrop.png");

    // Farmer
    int farmerWidth = 50;
    int farmerHeight = 70;

    std::string farmerSpriteNoHandsSheet = "entity/player/FarmerNoHands.png";
    farmerSpritesWithoutHands[EntityWarm::Action::IDLE].startFrame = 0;
    farmerSpritesWithoutHands[EntityWarm::Action::IDLE].endFrame = 1;
    loadTexture(farmerSpriteNoHandsSheet, farmerSpritesWithoutHands, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::IDLE);

    farmerSpritesWithoutHands[EntityWarm::Action::WALKING].startFrame = 2;
    farmerSpritesWithoutHands[EntityWarm::Action::WALKING].endFrame = 5;
    loadTexture(farmerSpriteNoHandsSheet, farmerSpritesWithoutHands, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::WALKING);

    std::string farmerSpriteWithHandsSheet = "entity/player/FarmerWithHands.png";
    farmerSpritesWithHands[EntityWarm::Action::IDLE].startFrame = 0;
    farmerSpritesWithHands[EntityWarm::Action::IDLE].endFrame = 1;
    loadTexture(farmerSpriteWithHandsSheet, farmerSpritesWithHands, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::IDLE);

    farmerSpritesWithHands[EntityWarm::Action::WALKING].startFrame = 2;
    farmerSpritesWithHands[EntityWarm::Action::WALKING].endFrame = 5;
    loadTexture(farmerSpriteWithHandsSheet, farmerSpritesWithHands, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::WALKING);

    // Alpaca
    std::string alpacaSpriteSheet = "entity/alpaca/alpaca-sprite.png";

    int alpacaWidth = 100;
    int alpacaHeight = 110;

    alpacaSprites[EntityWarm::Action::IDLE].startFrame = 0;
    alpacaSprites[EntityWarm::Action::IDLE].endFrame = 3;
    loadTexture(alpacaSpriteSheet, alpacaSprites, alpacaWidth, alpacaHeight, 0, EntityWarm::Action::IDLE);

    alpacaSprites[EntityWarm::Action::WALKING].startFrame = 0;
    alpacaSprites[EntityWarm::Action::WALKING].endFrame = 1;
    loadTexture(alpacaSpriteSheet, alpacaSprites, alpacaWidth, alpacaHeight, 0, EntityWarm::Action::WALKING);


    // Wolf
    std::string wolfSpriteSheet = "entity/wolf/wolf-sprite2.png";

    int wolfWidth = 145;
    int wolfHeight = 80;

    wolfSprites[EntityWarm::Action::IDLE].startFrame = 0;
    wolfSprites[EntityWarm::Action::IDLE].endFrame = 0;
    loadTexture(wolfSpriteSheet, wolfSprites, wolfWidth, wolfHeight, 95, EntityWarm::Action::IDLE);

    wolfSprites[EntityWarm::Action::WALKING].startFrame = 0;
    wolfSprites[EntityWarm::Action::WALKING].endFrame = 6;
    loadTexture(wolfSpriteSheet, wolfSprites, wolfWidth, wolfHeight, 95, EntityWarm::Action::WALKING);

}

void ConfigGame::reset() {

    // Initiating World (With no innate gravitation)
    delete world;
    world = new b2World(b2Vec2(0, 0));
    world->SetContactListener(new CollisionListener());

    // Instantiating initial entities
    delete planet;
    planet = new Planet(this, planetRadius, planetCenter.x, planetCenter.y);

    delete farmer;
    farmer = new Farmer(this, 30, 100, 100, calcX(0), calcY(0));

    delete entities;
    entities = new std::vector<Entity *>;
    entities->push_back(planet);
    entities->push_back(farmer);

    // Instantiating initial scenery
    delete sceneries;
    sceneries = new std::vector<Scenery *>;

    sky = new Sky(this, 4500.f, planetCenter.x, planetCenter.y);
    sceneries->push_back(sky);

    sun = new Sun(this, sunRadius, calcX(0.f, sunRadius), calcY(0.f, sunRadius), 0.f);
    sceneries->push_back(sun);

    // Initiate dayCycle
    delete dayCycle;
    dayCycle = new DayCycle(this);

    // Set ID
    Wolf::nextId = 0;
    Alpaca::nextId = 0;

}

ConfigGame::Cycle ConfigGame::getCurrentCycle() const {
    return currentCycle;
}

void ConfigGame::setCurrentCycle(ConfigGame::Cycle currentCycle) {
    ConfigGame::currentCycle = currentCycle;
}
