
#ifndef ALPACGAME_STATEPAUSE_H
#define ALPACGAME_STATEPAUSE_H

#include "State.h"
#include "../Resources/ConfigGame.h"
#include <TGUI/TGUI.hpp>

/**
 * The state that handles pausing of the game.
 * The pause state works in this manner, not ordered in any specific way:
 *      a. Transition to StatePause will ALWAYS pause the game.
 *      b. Pause is over when a new state is queued up.*
 *          -> * = Not when the next state is Option State.
 *      c. Pause State is basically a Game State, but with limited,
 *         possible inputs.
 */
class StatePause : public State{

public:

    /**
     * Run paused version of the StateGame.
     * @param stateMachine reference to StateMachine.
     */
    void goNext(StateMachine &stateMachine) override;


private:

    /**
     * Poll for event and state change.
     * @return true if the state remains, false if the state has changed.
     */
    bool pollGame();


    /// Pointers
    StateMachine *stateMachine = nullptr;
    ConfigGame *configGame = nullptr;
    sf::RenderWindow *window = nullptr;
    tgui::Gui *menuGUI = nullptr;
};


#endif //ALPACGAME_STATEPAUSE_H
