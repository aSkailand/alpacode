#include "StateExit.h"

void StateExit::goNext(StateMachine &stateMachine) {
    std::cout << "Exit State" << std::endl;
    stateMachine.isRunning = false;
}
