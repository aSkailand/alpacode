#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "../state/StateGame.h"


class planetClass {
public:
    planetClass(sf::RenderWindow &renderWindow);
    void control(float rotationDelta, StateGame::Direction rotationDir);
    void draw();

    StateGame::Direction currentDir = StateGame::Direction::RIGHT;

private:
    sf::RenderWindow *window;
    sf::VideoMode windowSize;

    sf::CircleShape planet;
    sf::RectangleShape alpaca;
    sf::RectangleShape alpaca2;

    sf::Sprite backgroundSprite;
    sf::Texture planetTexture;
    sf::Texture alpacaTexture;

    sf::Texture backgroundTexture;
    const int radius = 300;
    const int offset = -100; // Change this to change planet's y-pos
    double xpos = 0;
    double ypos = 0;
};

#endif