
#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"
#include "../../state/StateGame.h"

class Farmer {
public:

    enum class Action{IDLE, WALKING, JUMP};
    enum class Status{GROUNDED, AIRBORNE};
    enum class Direction{RIGHT, LEFT};

    Farmer(StateMachine &stateMachine, float initAngle);

    sf::RectangleShape farmer;

    float x,y;
    float speed = 50;

    float angle;

    void control();

    void draw();

    void loadTextures();

    Action action;
    Status status;
    Direction direction;


private:
    const int size = 100;

    ConfigGame *configGame;
    sf::RenderWindow *window;
    sf::Texture farmerTexture;
};


#endif //ALPACGAME_FARMER_H
