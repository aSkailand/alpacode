
#include "StateInitial.h"
#include "StateMachine.h"

void StateInitial::goNext(StateMachine &stateMachine) {

    stateMachine.configWindow.run();

    stateMachine.configSound.run();

    stateMachine.configMenu = new ConfigMenu;
    stateMachine.configMenu->run(stateMachine);

    stateMachine.configGame.run(stateMachine.configWindow.getWindow());

    stateMachine.setCurrentState(StateMachine::stateID::MENU);

}
