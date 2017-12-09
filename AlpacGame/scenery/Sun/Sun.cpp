//
// Created by Trong on 07/12/2017.
//

#include "Sun.h"
#include "../../Resources/ConfigGame.h"

Sun::Sun(ConfigGame *configGame, float radius, float x, float y, float angle) {

    this->configGame = configGame;

    sf_Shape = new sf::CircleShape(radius);
    sf_Shape->setOrigin(radius, radius);
    sf_Shape->setTexture(&configGame->sunTextures[0]);

    setPlacement(x,y,angle);
}

void Sun::render(sf::RenderWindow *window) {

    if(configGame->showLabels) sf_Shape->setOutlineThickness(2);
    else sf_Shape->setOutlineThickness(0);
    window->draw(*sf_Shape);
}

void Sun::update() {

}

void Sun::setPlacement(float x, float y, float angle) {
    sf_Shape->setPosition(x, y);
    sf_Shape->setRotation(angle);
}

void Sun::setTexture(sf::Texture *texture) {
    sf_Shape->setTexture(texture);
}
