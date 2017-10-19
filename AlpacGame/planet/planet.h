
#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../state/StateGame.h"

class Planet {
public:
    Planet(StateMachine &stateMachine);

    void draw();

    sf::CircleShape planet;

private:

    void loadTextures();
    StateMachine *machine;
    sf::RenderWindow *window;

    sf::Sprite backgroundSprite;
    sf::Texture planetTexture;
    sf::Texture backgroundTexture;

};

#endif