
#ifndef ALPACGAME_STATE_H
#define ALPACGAME_STATE_H

class StateMachine;

class State{
public:
    virtual void goNext(StateMachine &stateMachine) = 0;
};

#endif //ALPACGAME_STATE_H
