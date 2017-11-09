
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

void ConfigGame::loadTexture(std::string filename, std::map<EntityWarm::Action, AnimationInfo> &sprites, int mode,int width,int height, int y)
{

    /** mode: 0 = Action: Walking
     *  mode: 1 = Action: Idle
     */
    if(mode == 0)
    {

        for (int x = sprites[EntityWarm::Action::WALKING].startFrame; x < sprites[EntityWarm::Action::WALKING].endFrame; ++x) {

            sf::Texture* texture = new sf::Texture();

            if(!texture->loadFromFile(filename,sf::IntRect{x*width,y*height, width,height})){
                std::cout <<"Error loading file!"<< std::endl;
            }
            sprites[EntityWarm::Action::WALKING].sprites.push_back(texture);

        }
    }
    else if(mode == 1){
        // Sprites Action::IDLE
        for (int x = sprites[EntityWarm::Action::IDLE].startFrame; x < sprites[EntityWarm::Action::IDLE].endFrame ; ++x) {
            sf::Texture* texture = new sf::Texture();

            if(!texture->loadFromFile(filename,sf::IntRect{x*width,y*height, width,height})){
                std::cout <<"Error loading file!"<< std::endl;
            }
            sprites[EntityWarm::Action::IDLE].sprites.push_back(texture);
        }
    }
}

void ConfigGame::loadAllTextures() {
    /// Simply adding all sprites in a map, to easily getting it later

    /// wolf sizes: walking, width = 145, height = 80
    /// wolf sizes: idle, width = 135, height = 95
    /// alpaca sizes: width = 100, height = 110
    /// farmer sizes: width = 50, height = 70

    std::string wolfSpriteSheet = "entity/wolf/wolf-sprite.png";
    wolfSprites[EntityWarm::Action::WALKING].startFrame = 0;
    wolfSprites[EntityWarm::Action::WALKING].endFrame = 8;
    loadTexture(wolfSpriteSheet,wolfSprites,0,145,80,95);

    wolfSprites[EntityWarm::Action::IDLE].startFrame = 0;
    wolfSprites[EntityWarm::Action::IDLE].endFrame = 7;
    loadTexture(wolfSpriteSheet,wolfSprites,1,135,95,0);

    std::string alpacaSpriteSheet = "entity/alpaca/alpaca-sprite.png";
    alpacaSprites[EntityWarm::Action::WALKING].startFrame = 0;
    alpacaSprites[EntityWarm::Action::WALKING].endFrame = 5;
    loadTexture(alpacaSpriteSheet,alpacaSprites,0,100,110,0);

    std::string farmerSpriteSheet = "entity/player/farmer-sprite.png";
    farmerSprites[EntityWarm::Action::WALKING].startFrame = 0;
    farmerSprites[EntityWarm::Action::WALKING].endFrame = 7;
    loadTexture(farmerSpriteSheet,farmerSprites,0,50,70,0);

    // Getting the starting sprite.
    planetTexture.loadFromFile("entity/planet/planet.png");
    wolfTexture.loadFromFile("entity/wolf/wolf-sprite.png", sf::IntRect{0,0,135,95});
    alpacaTexture.loadFromFile("entity/alpaca/alpaca-sprite.png",sf::IntRect{0,0,100,110});
    farmerTexture.loadFromFile("entity/player/farmer-sprite.png",sf::IntRect{0,0,50,70});






    //wolfTexture.loadFromFile("entity/wolf/wolfy.png");
}
