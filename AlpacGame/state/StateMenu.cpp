#include "StateMenu.h"

void StateMenu::goNext(StateMachine &stateMachine) {

    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    window->setView(sf::View(window->getDefaultView()));
    menuGUI = machine->configWindow.getMenuGUI();

    /// TGUI setup
    menuGUI->removeAllWidgets();
    menuGUI->add(machine->configMenu->getPictureTittle());
    menuGUI->add(machine->configMenu->mapLayouts[ConfigMenu::layouts::MAINMENU]);


    if (!machine->configSound.isMenuMusicPlaying ) {
        machine->configSound.playMenuMusic(true);
    }

    window->setMouseCursorVisible(true);

    // Disable resume button if defeated
    if(machine->configGame.defeated){
        machine->configMenu->mapButtons[ConfigMenu::buttonID::RESUME]->disable();
        machine->configMenu->mapButtons[ConfigMenu::buttonID::RESUME]->setOpacity(0.5f);
    } else{
        machine->configMenu->mapButtons[ConfigMenu::buttonID::RESUME]->enable();
        machine->configMenu->mapButtons[ConfigMenu::buttonID::RESUME]->setOpacity(1.f);
    }

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
    menuGUI->draw();
    window->display();
}
