#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>
//#include <SFML/System/Export.hpp>

//#include "../../Resources/Config.h"


struct Alpaca
{
public:
    Alpaca(sf::RenderWindow& window);

    const int size = 50;
    const float speed = 400;
    float x, y;

    sf::RectangleShape alpaca;

    enum class Direction{LEFT, RIGHT};
    enum class Action{IDLE, WALKING};


    void move();
    void draw();
    void randomAction();

    //Add what happens when you are direction = LEFT, RIGHT and same for Action
    //Make randomAction happen every 3 sec

private:

    sf::RenderWindow *window;

    int actionTick = 3;

    sf::Texture alpacaTexture;

    Direction currentDirection;
    Action currentAction;

    sf::Clock clock;
    sf::Time currentTime;
};

#endif //ALPACGAME_ALPACA_H
