#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <SFML/Graphics.hpp>
#include <iostream>



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

    void setDirectionState(Direction setDirection);

    void setActionState(Action setAction);

private:

    Direction currentDirectionState;

    Action currentActionState;

    int tickSecond = 3;
public:
    int getTickSecond() const;

private:
    bool moveWolf;

};


#endif //ALPACGAME_WOLFSTATE_H
