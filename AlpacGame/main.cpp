#include <iostream>
#include <plog\log.h>
#include <SFML/Graphics.hpp>
#include "state/StateMachine.h"

int main() {

    plog::init(plog::debug,"Log.log");
    LOGI << "Initiating plog.";

    StateMachine stateMachine;

    while (stateMachine.isRunning){
        std::cout << "Machine is Proceeding:" << std::endl;
        stateMachine.proceed();
    }

    return 0;
}