#include "StateOption.h"

void StateOption::goNext(StateMachine &stateMachine) {

    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    menuGUI = machine->configWindow.getMenuGUI();

    /// TGUI setup
    menuGUI->removeAllWidgets();
    menuGUI->add(machine->configMenu.getPictureMenu());
    menuGUI->add(machine->configMenu.mapLayouts.find(ConfigMenu::MAP_LAYOUTS::SETTINGS)->second);

    while (PollOption()) {
        drawOption();
    }
}

bool StateOption::PollOption() {
    sf::Event event;
    window->pollEvent(event);
    if (event.type == sf::Event::Closed) {
        window->close();
        machine->setCurrentState(StateMachine::stateID::EXIT);
        return false;
    }
    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::OPTION;
}

void StateOption::drawOption() {
    window->clear(sf::Color::Black);
    menuGUI->draw();
    window->display();
}

