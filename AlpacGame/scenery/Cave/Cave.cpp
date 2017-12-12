
#include <chrono>
#include "Cave.h"

Cave::Cave(ConfigGame *configGame, float width, float height) {

    this->configGame = configGame;

    sf_Shape = new sf::RectangleShape(sf::Vector2f(width, height));
    sf_Shape->setOrigin(width/2, height);
    sf_Shape->setTexture(&configGame->trapOpenTexture);

    configGame->sceneries->push_back(this);
}

void Cave::render(sf::RenderWindow *window) {
    window->draw(*sf_Shape);
}


void Cave::update() {

}

void Cave::reposition(float angle) {
    sf_Shape->setPosition(configGame->calcX(angle), configGame->calcY(angle));
    sf_Shape->setRotation(angle);
}
