
#include "StateInitial.h"
#include "StateMachine.h"

void StateInitial::goNext(StateMachine &stateMachine){

    stateMachine.configWindow.run();
    stateMachine.configGame.run(stateMachine.configWindow.getWindow());

    stateMachine.setCurrentState(StateMachine::stateID::MENU);

}
