
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

void ConfigGame::loadTexture(std::string filename, std::map<EntityWarm::Action, AnimationInfo> &sprites,int width,int height, int top, int mode)
{

    /** mode: 0 = Action: Walking
     *  mode: 1 = Action: Idle
     */
    if(mode == 0)
    {

        for (int x = sprites[EntityWarm::Action::WALKING].startFrame; x < sprites[EntityWarm::Action::WALKING].endFrame; ++x) {

            sf::Texture* texture = new sf::Texture();

            if(!texture->loadFromFile(filename,sf::IntRect{x*width, top, width, height})){
                std::cout <<"Error loading file!"<< std::endl;

            }
            sprites[EntityWarm::Action::WALKING].sprites.push_back(texture);

        }
    }
    else if(mode == 1){
        // Sprites Action::IDLE
        for (int x = sprites[EntityWarm::Action::IDLE].startFrame; x < sprites[EntityWarm::Action::IDLE].endFrame ; ++x) {
            sf::Texture* texture = new sf::Texture();

            if(!texture->loadFromFile(filename,sf::IntRect{x*width, top, width, height})){
                std::cout <<"Error loading file!"<< std::endl;
            }
            sprites[EntityWarm::Action::IDLE].sprites.push_back(texture);
        }
    }
    else if(mode == 2){
        for (int x = sprites[EntityWarm::Action::JUMP].startFrame; x < sprites[EntityWarm::Action::JUMP].endFrame ; ++x) {
            sf::Texture* texture = new sf::Texture();

            if(!texture->loadFromFile(filename,sf::IntRect{x*width, top, width, height})){
                std::cout <<"Error loading file!"<< std::endl;
            }
            sprites[EntityWarm::Action::JUMP].sprites.push_back(texture);
        }
    }
}

void ConfigGame::loadAllTextures() {
    /// Simply adding all sprites in a map, to easily getting it later

    /// wolf sizes: walking, width = 145, height = 80
    /// wolf sizes: idle, width = 135, height = 95
    /// alpaca sizes: width = 100, height = 110
    /// farmer sizes: width = 50, height = 70

    /*std::string wolfSpriteSheet = "entity/wolf/wolf-sprite.png";
    wolfSprites[EntityWarm::Action::WALKING].startFrame = 0;
    wolfSprites[EntityWarm::Action::WALKING].endFrame = 6;
    loadTexture(wolfSpriteSheet,wolfSprites,wolfWalkWidth,wolfWalkHeight,95,0);

    wolfSprites[EntityWarm::Action::IDLE].startFrame = 0;
    wolfSprites[EntityWarm::Action::IDLE].endFrame = 6;
    loadTexture(wolfSpriteSheet,wolfSprites,wolfIdleWidth,wolfIdleheigth,0,1);
*/
    std::string alpacaSpriteSheet = "entity/alpaca/alpaca-sprite.png";
    alpacaSprites[EntityWarm::Action::WALKING].startFrame = 0;
    alpacaSprites[EntityWarm::Action::WALKING].endFrame = 2;
    loadTexture(alpacaSpriteSheet,alpacaSprites,alpacaWidth,alpacaHeight,0,0);

    alpacaSprites[EntityWarm::Action::IDLE].startFrame = 0;
    alpacaSprites[EntityWarm::Action::IDLE].endFrame = 4;
    loadTexture(alpacaSpriteSheet,alpacaSprites,alpacaWidth,alpacaHeight,0,1);

    std::string farmerSpriteSheet = "entity/player/farmer-sprite.png";
    farmerSprites[EntityWarm::Action::WALKING].startFrame = 0;
    farmerSprites[EntityWarm::Action::WALKING].endFrame = 6;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,0);

    farmerSprites[EntityWarm::Action::IDLE].startFrame = 0;
    farmerSprites[EntityWarm::Action::IDLE].endFrame = 1;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,1);

    farmerSprites[EntityWarm::Action::IDLE].startFrame = 3;
    farmerSprites[EntityWarm::Action::IDLE].endFrame = 4;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,1);

    farmerSprites[EntityWarm::Action::JUMP].startFrame = 0;
    farmerSprites[EntityWarm::Action::JUMP].endFrame = 6;
    loadTexture(farmerSpriteSheet,farmerSprites,farmerWidth,farmerHeight,0,2);

    // Getting the starting sprite.
    planetTexture.loadFromFile("entity/planet/planet.png");
    wolfTexture.loadFromFile("entity/wolf/wolfy.png");
    alpacaTexture.loadFromFile("entity/alpaca/alpaca-sprite.png",sf::IntRect{0,0,100,110});
    //farmerTexture.loadFromFile("entity/player/farmer-sprite.png",sf::IntRect{0,0,50,70});

}
