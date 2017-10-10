
#ifndef ALPACGAME_STATEGAME_H
#define ALPACGAME_STATEGAME_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "State.h"
#include "StateMachine.h"

class StateGame : public State{
public:
    void goNext(StateMachine &stateMachine);
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
