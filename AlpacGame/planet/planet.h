
#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../state/StateGame.h"


class Planet {
public:
    Planet(StateMachine &stateMachine
    );
    void control(float rotationDelta);
    void draw();


private:

    void loadTextures();

    StateMachine *machine;
    sf::RenderWindow *window;
    sf::VideoMode windowSize;

    sf::CircleShape planet;

    sf::Sprite backgroundSprite;
    sf::Texture planetTexture;
    sf::Texture backgroundTexture;

    int radius;
    int offset;
    double xpos = 0;
    double ypos = 0;
};

#endif