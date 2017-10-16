#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <random>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>

class WolfState {
public:
    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action {
        IDLE, WALKING
    };

    Action getCurrentActionState();

    void goWolf(int elapsedTime);

    int randomNumberGenerator();

    int getTickSecond() const;

private:
    Direction currentDirectionState;
    Action currentActionState;
    int tickSecond = 3;
};

#endif //ALPACGAME_WOLFSTATE_H
