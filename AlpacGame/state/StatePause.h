
#ifndef ALPACGAME_STATEPAUSE_H
#define ALPACGAME_STATEPAUSE_H

#include "State.h"
#include "../Resources/ConfigGame.h"
#include <TGUI/TGUI.hpp>

/**
 * The state that handles pausing of the game.
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
    tgui::Gui *menuGUI;
};


#endif //ALPACGAME_STATEPAUSE_H
