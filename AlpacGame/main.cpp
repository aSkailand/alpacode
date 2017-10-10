
#include <iostream>
#include <plog\log.h>
#include <SFML/Graphics.hpp>
#include "state/StateMachine.h"

int main()
{

    // Initiating Plog
    plog::init(plog::debug,"Log.log");
    LOGI << "Initiating plog.";


    // Initiating StateMachine
    StateMachine stateMachine;

    // Proceed if StateMachine is still on
    while (stateMachine.isRunning){
        std::cout << "Machine is Proceeding:" << std::endl;
        stateMachine.proceed();
    }

    return 0;
}