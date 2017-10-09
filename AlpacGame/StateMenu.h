
#ifndef ALPACGAME_STATEMENU_H
#define ALPACGAME_STATEMENU_H


#include "State.h"
#include "StateMachine.h"

class StateMenu : public State{
public:
    void goNext(StateMachine &stateMachine);
};


#endif //ALPACGAME_STATEMENU_H
