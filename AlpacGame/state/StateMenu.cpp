#include "StateMenu.h"

void StateMenu::goNext(StateMachine &stateMachine) {

    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    window->setView(sf::View(window->getDefaultView()));
    menuGUI = machine->configWindow.getMenuGUI();

    /// TGUI setup
    menuGUI->removeAllWidgets();
    menuGUI->add(machine->configMenu->getPictureMenu());
    menuGUI->add(machine->configMenu->mapLayouts[ConfigMenu::layouts::MAINMENU]);

    while (pollMenu()) {
        drawMenu();
    }
}

bool StateMenu::pollMenu() {
    sf::Event event;
    window->pollEvent(event);
    if (event.type == sf::Event::Closed) {
        machine->setCurrentState(StateMachine::stateID::EXIT);
    }
    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::MENU;
}

void StateMenu::drawMenu() {
    window->clear(sf::Color::Black);
    menuGUI->draw();
    window->display();
}
