
#ifndef ALPACGAME_STATEGAME_H
#define ALPACGAME_STATEGAME_H


#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "State.h"
#include "StateMachine.h"

#include "../planet/planet.h"
#include "../entity/player/Farmer.h"
#include "../wolf/WolfState.h"
#include "../../entity/alpaca/alpaca.h"

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
