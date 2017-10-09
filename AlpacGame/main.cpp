
#include <iostream>
#include <plog\log.h>
#include <SFML/Graphics.hpp>
#include "window.h"
#include "state/StateMachine.h"

int main() {

    plog::init(plog::debug,"Log.log");
    LOGI << "Initiating plog.";

    StateMachine stateMachine;

    while (stateMachine.isRunning){
        std::cout << "Machine is Proceeding:" << std::endl;
        stateMachine.proceed();
    }

    window window;

    return 0;
}