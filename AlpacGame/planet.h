#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>


class planetClass {
public:
    void planet_control();
private:
    const int radius = 250;
    const int rotationSpeed = 100;
    const int offset = -200; // Change this to change planet's y-pos
    double xpos = 0;
    double ypos = 0;
    const sf::VideoMode windowSize = sf::VideoMode(800, 600);
};

#endif