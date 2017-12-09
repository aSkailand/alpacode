
#include "Sky.h"

Sky::Sky(ConfigGame *configGame, float radius, float x, float y) {

    this->configGame = configGame;

    shapeForeground = sf::CircleShape(radius);
    shapeForeground.setOrigin(shapeForeground.getRadius(), shapeForeground.getRadius());
    shapeForeground.setPosition(x,y);
    shapeForeground.setTexture(&configGame->skyTextures[0]);

    shapeBackground = sf::CircleShape(radius);
    shapeBackground.setOrigin(shapeBackground.getRadius(), shapeBackground.getRadius());
    shapeBackground.setPosition(x,y);
    shapeBackground.setTexture(&configGame->skyTextures[1]);

}

void Sky::render(sf::RenderWindow *window) {

    shapeForeground.setFillColor(shapeForeground.getFillColor() - sf::Color(0,0,0,1));

    window->draw(shapeBackground);
    window->draw(shapeForeground);
}

void Sky::update() {

}

void Sky::setTexture(unsigned int cycleFrame) {

    shapeForeground.setFillColor(shapeForeground.getFillColor() + sf::Color(0,0,0,255));

    shapeForeground.setTexture(&configGame->skyTextures[cycleFrame]);
    shapeBackground.setTexture(&configGame->skyTextures[(cycleFrame == 11) ? 0 : ++cycleFrame]);

}
