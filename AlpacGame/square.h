//
// Created by Bruker on 09/10/2017.
//

#ifndef ALPACGAME_SQUARE_H
#define ALPACGAME_SQUARE_H


#include <SFML/Graphics.hpp>

#include "config.h"


// Simple structure (a class where everything is public) for our square
struct Square
{
public:
    Square(float x, float y, const Config& config, sf::RenderWindow& window);
    void move(float delta);
    void draw();

    const int size = 100;
    const float speed = 400;
    float x, y, velx, vely, accx, accy;
    float grav = 3;

    enum class Action{IDLE, WALKING, JUMP};
    enum class Direction{RIGHT, LEFT};
    Action action;
    Direction direction;

    sf::RectangleShape rectangle;

protected:
    // Dependencies
    const Config& config;
    sf::RenderWindow& window;



};

#endif //ALPACGAME_SQUARE_H
