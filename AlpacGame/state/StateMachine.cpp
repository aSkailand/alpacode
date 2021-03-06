
#include "StateMachine.h"
#include "StatePause.h"


StateMachine::StateMachine() {

    stateMap.emplace(StateMachine::stateID::INITIAL, new StateInitial);
    stateMap.emplace(StateMachine::stateID::MENU, new StateMenu);
    stateMap.emplace(StateMachine::stateID::SINGLEPLAYER, new StateGame);
    stateMap.emplace(StateMachine::stateID::PAUSE, new StatePause);
    stateMap.emplace(StateMachine::stateID::OPTION, new StateOption);
    stateMap.emplace(StateMachine::stateID::EXIT, new StateExit);

    setCurrentState(stateID::INITIAL);

    StateMachine::isRunning = true;
}

void StateMachine::setCurrentState(StateMachine::stateID currentState) {
    StateMachine::currentState = currentState;
}

void StateMachine::proceed() {
    stateMap[currentState] -> goNext(*this);
}

StateMachine::stateID StateMachine::getCurrentState() const {
    return currentState;
}


