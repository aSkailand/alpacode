
#include <cmath>
#include <iostream>
#include "ConfigGame.h"

void ConfigGame::run(sf::RenderWindow &window) {
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

void ConfigGame::loadTexture(std::string filename, std::map<EntityWarm::Action, AnimationInfo> &sprites,int width,int height, int top, EntityWarm::Action action)
{
    /** EntityWarm::Action mode:
     *  WALKING
     *  IDLE
     *  JUMP
     */

    for (int x = sprites[action].startFrame; x < sprites[action].endFrame; ++x) {
        sf::Texture* texture = new sf::Texture();

        if(!texture->loadFromFile(filename,sf::IntRect{x*width, top, width, height})){
                std::cout <<"Error loading file!"<< std::endl;
        }
        sprites[action].sprites.push_back(texture);

    }
}

void ConfigGame::loadAllTextures() {
    /**
     * Simply adding all sprites in a map, to easily getting it later.
     * First entering numbers of sprites of differents mode, by given
     * startFrame and endFrame.
     * then add to an function where its need a spriteSheet string name,
     * width, height, Top, and mode.
     */

    /// wolf sizes: walking, width = 145, height = 80
    /// wolf sizes: idle, width = 135, height = 95
    /// alpaca sizes: width = 100, height = 110
    /// farmer sizes: width = 50, height = 70

    /// Adding sprites into vector for Wolf.
    std::string wolfSpriteSheet = "entity/wolf/wolf-sprite2.png";
    wolfSprites[EntityWarm::Action::WALKING].startFrame = 0;
    wolfSprites[EntityWarm::Action::WALKING].endFrame = 7;
    loadTexture(wolfSpriteSheet,wolfSprites,wolfWalkWidth,wolfWalkHeight,95,EntityWarm::Action::WALKING);

    wolfSprites[EntityWarm::Action::IDLE].startFrame = 0;
    wolfSprites[EntityWarm::Action::IDLE].endFrame = 1;
    loadTexture(wolfSpriteSheet,wolfSprites,wolfWalkWidth,wolfWalkHeight,95,EntityWarm::Action::IDLE);

    /// Adding sprites into vector for Alpaca.
    std::string alpacaSpriteSheet = "entity/alpaca/alpaca-sprite.png";
    alpacaSprites[EntityWarm::Action::WALKING].startFrame = 0;
    alpacaSprites[EntityWarm::Action::WALKING].endFrame = 2;
    loadTexture(alpacaSpriteSheet,alpacaSprites,alpacaWidth,alpacaHeight,0,EntityWarm::Action::WALKING);

    alpacaSprites[EntityWarm::Action::IDLE].startFrame = 0;
    alpacaSprites[EntityWarm::Action::IDLE].endFrame = 4;
    loadTexture(alpacaSpriteSheet,alpacaSprites,alpacaWidth,alpacaHeight,0,EntityWarm::Action::IDLE);

    /// Adding sprites into vector for Farmer.
    std::string farmerSpriteSheet = "entity/player/farmer-sprite.png";
    farmerSprites[EntityWarm::Action::WALKING].startFrame = 0;
    farmerSprites[EntityWarm::Action::WALKING].endFrame = 6;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,EntityWarm::Action::WALKING);

    farmerSprites[EntityWarm::Action::IDLE].startFrame = 0;
    farmerSprites[EntityWarm::Action::IDLE].endFrame = 1;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,EntityWarm::Action::IDLE);

    farmerSprites[EntityWarm::Action::IDLE].startFrame = 3;
    farmerSprites[EntityWarm::Action::IDLE].endFrame = 4;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,EntityWarm::Action::IDLE);

    farmerSprites[EntityWarm::Action::JUMP].startFrame = 0;
    farmerSprites[EntityWarm::Action::JUMP].endFrame = 6;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,EntityWarm::Action::JUMP);

    /// Getting planet graphic.
    planetTexture.loadFromFile("entity/planet/planet.png");

}
