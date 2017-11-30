
#include "StateInitial.h"
#include "StateMachine.h"

void StateInitial::goNext(StateMachine &stateMachine) {



    // Run ConfigWindow
    stateMachine.configWindow.run();

    // Run ConfigSound
    stateMachine.configSound.run();

    // Run ConfigMenu
    stateMachine.configMenu = new ConfigMenu;
    stateMachine.configMenu->run(stateMachine);

    // Run ConfigGame
    stateMachine.configGame.run(stateMachine.configWindow.getWindow());

    // Change State
    stateMachine.setCurrentState(StateMachine::stateID::MENU);

}
