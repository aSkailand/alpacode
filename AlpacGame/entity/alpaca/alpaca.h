#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"

struct Alpaca {
public:
    Alpaca(float initDegree, StateMachine &stateMachine);

    const int size = 50;
    const float speed = 400;
    float x, y;

    sf::RectangleShape alpaca;

    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action {
        IDLE, WALKING
    };


    void control(float deltaRotation);

    void draw();

    void randomAction();

    void randomDirection();

    //Add what happens when you are direction = LEFT, RIGHT and same for Action
    //Make randomAction happen every 3 sec

private:

    StateMachine *machine;
    sf::RenderWindow *window;

    int actionTick = 3;

    sf::Texture alpacaTexture;

    Direction currentDirection;
    Action currentAction;

    sf::Clock clock;
};

#endif //ALPACGAME_ALPACA_H
