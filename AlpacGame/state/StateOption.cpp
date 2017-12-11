#include "StateOption.h"

void StateOption::goNext(StateMachine &stateMachine) {

    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    menuGUI = machine->configWindow.getMenuGUI();

    /// TGUI setup
    menuGUI->removeAllWidgets();
    menuGUI->add(machine->configMenu->getPictureMenu());
    menuGUI->add(machine->configMenu->mapLayouts[ConfigMenu::layouts::SETTINGS]);

    while (PollOption()) {
        drawOption();
    }
}

bool StateOption::PollOption() {
    sf::Event event;
    window->pollEvent(event);
    if (event.type == sf::Event::Closed) {
        machine->setCurrentState(StateMachine::stateID::EXIT);
    }
    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::OPTION;
}

void StateOption::drawOption() {
    menuGUI->draw();
    window->display();
}

