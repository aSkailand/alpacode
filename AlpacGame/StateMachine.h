
#ifndef ALPACGAME_STATEMACHINE_H
#define ALPACGAME_STATEMACHINE_H


#include <map>
#include "State.h"
#include "StateMenu.h"
#include "StateExit.h"

class State;

class StateMachine {

public:

    enum class stateID { INITIAL, MENU, SINGLEPLAYER, PAUSE, EXIT};

    StateMachine();

    bool isRunning;

    void proceed();

    void setIsRunning(bool isRunning);

    stateID currentState = stateID::INITIAL;

    void setCurrentState(stateID currentState);

private:
    std::map<stateID, State*> stateMap;

};


#endif //ALPACGAME_STATEMACHINE_H
