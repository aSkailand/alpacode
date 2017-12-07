//
// Created by Trong on 07/12/2017.
//

#include "Sky.h"

Sky::Sky(ConfigGame *configGame, float radius, float x, float y) {
    sf_Shape = new sf::CircleShape(radius);
    sf_Shape->setOrigin(radius, radius);
    sf_Shape->setPosition(x, y);
    setTexture(&configGame->skyTextures[0]);
}

void Sky::render(sf::RenderWindow *window) {
    window->draw(*sf_Shape);
}

void Sky::update() {

}

void Sky::setTexture(sf::Texture *texture) {
    sf_Shape->setTexture(texture);
}
