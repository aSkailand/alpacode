
#include "StateExit.h"

void StateExit::goNext(StateMachine &stateMachine) {
    stateMachine.setIsRunning(false);
}
