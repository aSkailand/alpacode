#ifndef ALPACGAME_STATEMACHINE_H
#define ALPACGAME_STATEMACHINE_H

#include <map>
#include "State.h"
#include "StateInitial.h"
#include "StateMenu.h"
#include "StateGame.h"
#include "StateOption.h"
#include "StateExit.h"
#include "../Resources/ConfigWindow.h"
#include "../Resources/ConfigMenu.h"
#include "../Resources/ConfigGame.h"
#include "../Resources/ConfigSound.h"

class State;

class StateMachine {
public:
    /**
     * Keeps all the stateIDs for all possible states.
     */
    enum class stateID {
        INITIAL, MENU, SINGLEPLAYER, PAUSE, OPTION, EXIT
    };

    stateID getCurrentState() const;
    /**
     * Initiate starter values for the StateMachine, then turns itself on.
     */
    StateMachine();

    /**
     * Proceed to next state by accessing the current state's GoNext().
     */
    void proceed();

    /**
     * Bool keeping track if the StateMachine is running or not.
     */
    bool isRunning;

    /**
     * Setter for currentState.
     * @param currentState the new state to be in.
     */
    void setCurrentState(stateID currentState);

    ConfigWindow configWindow;
    ConfigMenu *configMenu = nullptr;
    ConfigGame configGame;
    ConfigSound configSound;

private:
    /**
     * The current state the StateMachine is running.
     */
    stateID currentState;

    /**
     * Mappings of stateIDs with corresponding states.
     */
    std::map<stateID, State *> stateMap;
};


#endif //ALPACGAME_STATEMACHINE_H
