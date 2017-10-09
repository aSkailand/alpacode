
#include "StateMachine.h"

StateMachine::StateMachine() {

    stateMap.emplace(StateMachine::stateID::MENU, new StateMenu);
    stateMap.emplace(StateMachine::stateID::EXIT, new StateExit);

    setCurrentState(stateID::MENU);

    StateMachine::isRunning = true;

}

void StateMachine::setCurrentState(StateMachine::stateID currentState) {
    StateMachine::currentState = currentState;
}

void StateMachine::proceed() {
    stateMap[currentState] -> goNext(*this);
}

