
#include "StateMachine.h"

StateMachine::StateMachine() {

    StateMachine::isRunning = true;
    setCurrentState(stateID::MENU);

    stateMap.emplace(StateMachine::stateID::MENU, new StateMenu);
    stateMap.emplace(StateMachine::stateID::EXIT, new StateExit);

}

void StateMachine::setCurrentState(StateMachine::stateID currentState) {
    StateMachine::currentState = currentState;
}

void StateMachine::proceed() {
    stateMap[currentState] -> goNext(*this);
}

void StateMachine::setIsRunning(bool isRunning) {
    StateMachine::isRunning = isRunning;
}
