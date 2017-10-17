
#ifndef ALPACGAME_STATEGAME_H
#define ALPACGAME_STATEGAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "State.h"
#include "StateMachine.h"
#include "../Resources/ConfigWindow.h"
#include "../Resources/ConfigGame.h"

#include "../planet/planet.h"
#include "../entity/player/Farmer.h"
#include "../wolf/WolfState.h"
#include "../entity/alpaca/Alpaca.h"

class StateGame : public State{
public:

    void goNext(StateMachine &stateMachine);

private:

    // Pointers
    StateMachine *machine;
    ConfigGame *configGame;
    sf::RenderWindow *window;

    // View
    sf::View view;
    float viewZoom = 1.0f;  // How much to zoom in
    int viewOffset = 400;   // How much is the view's offset

    // Game State Functions
    /**
     * Check if game is ongoing.
     * @return true if game is still running, false if an call of exit is done.
     */
    bool pollGame();

};


#endif //ALPACGAME_STATEGAME_H
