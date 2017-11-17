
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
}
