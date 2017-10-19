#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <random>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

class Wolf {
public:

    Wolf(StateMachine &stateMachine, float initAngle);

    //constructor dependancy inject render window
    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action{
        IDLE, WALKING
    };

    Action getCurrentActionState();
    void control();
    void draw();
    int randomNumberGenerator(int lower, int upper);
    int getTickSecond() const;

    float x;
    float y;
    float angle;

    sf::Clock clock;
    sf::Time elapsedTime;
    sf::RectangleShape wolfRectangle;


private:
    const int size = 100;
    Direction currentDirectionState;
    Action currentActionState;
    int tickSecond = 3;


    ConfigGame *configGame;
    sf::RenderWindow *window;
    sf::VideoMode windowSize;
    sf::Texture wolfTexture;


};

#endif //ALPACGAME_WOLFSTATE_H
