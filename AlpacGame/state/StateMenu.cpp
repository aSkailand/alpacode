
#include <iostream>
#include "StateMenu.h"

void StateMenu::goNext(StateMachine &stateMachine) {

    std::string temp;
    std::cout << "Write String" << std::endl;
    std::cin >> temp;

    while(temp != "exit"){
        std::cout << "Write String" << std::endl;
        std::cin >> temp;
    }

    stateMachine.setCurrentState(StateMachine::stateID::EXIT);


}
