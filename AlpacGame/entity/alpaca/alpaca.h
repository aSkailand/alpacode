#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H


#include "../../Resources/Config.h"
#include <SFML/System/Export.hpp>
#include <random>
#include <iostream>


struct Alpaca
{
public:
    Alpaca(float x, float y, const Config& config, sf::RenderWindow& window);

    const int size = 50;
    const float speed = 400;
    float x, y;

    sf::RectangleShape rectangle;

    enum class Direction{LEFT, RIGHT};
    enum class Action{IDLE, WALKING};


    void move(float delta);
    void draw();
    void randomAction();

    //Add what happens when you are direction = LEFT, RIGHT and same for Action
    //Make randomAction happen every 3 sec


    // Dependencies
    const Config& config;
    sf::RenderWindow& window;


    Direction getCurrentDirection() const;
    Action getCurrentAction() const;

private:
    Direction currentDirection;
    Action currentAction;
};

#endif //ALPACGAME_ALPACA_H
