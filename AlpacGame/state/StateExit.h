//
// Created by Trong on 09/10/2017.
//

#ifndef ALPACGAME_STATEEXIT_H
#define ALPACGAME_STATEEXIT_H

// TODO: DELETE, THIS CLASS A TESTER

#include "State.h"
#include "StateMachine.h"

class StateExit : public State{
public:
    void goNext(StateMachine &stateMachine);
};


#endif //ALPACGAME_STATEEXIT_H
