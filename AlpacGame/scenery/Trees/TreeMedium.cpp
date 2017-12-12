#include "TreeMedium.h"

TreeMedium::TreeMedium(ConfigGame *configGame, float width, float height, float angle) {

    this->configGame = configGame;

    sf_Shape = new sf::RectangleShape(sf::Vector2f(width,height));
    sf_Shape->setOrigin(width/2, height);
    sf_Shape->setTexture(&configGame->treeMediumTexture);
    sf_Shape->setPosition(configGame->calcX(angle), configGame->calcY(angle));
    sf_Shape->setRotation(angle);

}

void TreeMedium::render(sf::RenderWindow *window){

    window->draw(*sf_Shape);
}

void TreeMedium::update() {

}