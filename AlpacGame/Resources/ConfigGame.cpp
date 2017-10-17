
#include <cmath>
#include "ConfigGame.h"

void ConfigGame::run(sf::RenderWindow &window) {
    planetCenter = sf::Vector2f(window.getSize().x/2,window.getSize().y - offset);
    planetRadius = 300;

}

float ConfigGame::calcX(float degree) {
    float x = planetCenter.x + planetRadius * (float) std::sin(degree*(M_PI/180));
    return x;
}

float ConfigGame::calcY(float degree) {
    float y = planetCenter.y - planetRadius * (float) std::cos(degree*(M_PI/180));
    return y;
}
