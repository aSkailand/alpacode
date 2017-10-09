
#include "StateInitial.h"
#include "StateMachine.h"

void StateInitial::goNext(StateMachine &stateMachine){
    stateMachine.config.run();
    stateMachine.setCurrentState(StateMachine::stateID::MENU);
}
