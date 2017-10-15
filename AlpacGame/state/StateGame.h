
#ifndef ALPACGAME_STATEGAME_H
#define ALPACGAME_STATEGAME_H


#include <SFML/Graphics/RenderWindow.hpp>

#include "State.h"
#include "StateMachine.h"

#include "../planet/planet.h"
#include "../entity/player/Farmer.h"

class StateGame : public State{
public:

    enum class Direction{ LEFT, RIGHT };

    void goNext(StateMachine &stateMachine);

    float rotationDelta;
    int rotationSpeed;

private:

    // Pointers
    StateMachine *machine;
    sf::RenderWindow *window;

    // Game State Functions
    /**
     * Check if game is ongoing.
     * @return true if game is still running, false if an call of exit is done.
     */
    bool pollGame();

};


#endif //ALPACGAME_STATEGAME_H
