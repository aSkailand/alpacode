#include <iostream>
#include <SFML/Graphics.hpp>
#include "state/StateMachine.h"

int main() {

    StateMachine stateMachine;

    while (stateMachine.isRunning){
        std::cout << "Machine is Proceeding:" << std::endl;
        stateMachine.proceed();
    }

    return 0;
}