#include "StateExit.h"

void StateExit::goNext(StateMachine &stateMachine) {
    stateMachine.isRunning = false;
}
