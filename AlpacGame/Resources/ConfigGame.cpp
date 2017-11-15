
#include <cmath>
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

void ConfigGame::loadAllTextures() {
    planetTexture.loadFromFile("entity/planet/planet.png");
    farmerTexture.loadFromFile("entity/player/farmer.png");
    alpacaTexture.loadFromFile("entity/alpaca/alpaca.png");
    wolfTexture.loadFromFile("entity/wolf/wolfy.png");
    alertRedTexture.loadFromFile("entity/alpaca/redAlert.png");
    alertYelTexture.loadFromFile("entity/alpaca/yellowAlert.png");
}
