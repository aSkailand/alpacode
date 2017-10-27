#include "StateMenu.h"

void StateMenu::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    gui = &machine->configWindow.getGUI();


    window->setView(sf::View(window->getDefaultView()));

    initMenuStrings();

    while (pollMenu()) {
        drawMenu();
        gui->draw();
    }
}

bool StateMenu::pollMenu() {
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed : {
                machine->setCurrentState(StateMachine::stateID::EXIT);
                return false;
            }
            case sf::Event::KeyPressed : {
                switch (event.key.code) {
                    case sf::Keyboard::Up : {
                        moveUp();
                        return true;
                    }
                    case sf::Keyboard::Down : {
                        moveDown();
                        return true;
                    }
                    case sf::Keyboard::Return : {
                        switch (menuIndex) {
                            case 0 : {
                                /**
                                 * Changes state to game
                                 */
                                machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER);
                                return false;
                            }

                            case 1 : {
                                /**
                                 * Changes state to option menu
                                 */
                                machine->setCurrentState(StateMachine::stateID::OPTION);
                                return false;
                            }

                            case 2 : {
                                /**
                                 * Exits the game
                                 */
                                machine->setCurrentState(StateMachine::stateID::EXIT);
                                return false;
                            }

                        }
                    }
                }
                break;
            }

        }

        gui->handleEvent(event);


    }
    return true;
}

void StateMenu::drawMenu() {

    window->clear(sf::Color::Black);

    for (const sf::Text &text : menuChoices) {
        window->draw(text);
    }

    window->display();
}

void StateMenu::initMenuStrings() {

    fontMenuChoice.loadFromFile("resources/fontMenu.ttf");

    menuChoices.clear();    // todo remove if adding constructor

    addMenuChoice("Play", 350, 150);
    addMenuChoice("Option", 350, 250);
    addMenuChoice("Exit", 350, 350);

    menuIndex = 0;
    menuChoices[0].setFillColor(colorSelected);

}

void StateMenu::moveUp() {
    if (menuIndex - 1 >= 0) {
        menuChoices[menuIndex].setFillColor(colorDeselected);
        menuIndex--;
        menuChoices[menuIndex].setFillColor(colorSelected);
    }
}

void StateMenu::moveDown() {
    if (menuIndex + 1 < menuChoices.size()) {
        menuChoices[menuIndex].setFillColor(colorDeselected);
        menuIndex++;
        menuChoices[menuIndex].setFillColor(colorSelected);
    }
}

void StateMenu::addMenuChoice(const std::string &choice, int x, int y) {
    sf::Text temp = sf::Text(choice, fontMenuChoice, fontSizeMenuChoice);
    temp.setPosition(x, y);
    temp.setFillColor(colorDeselected);
    menuChoices.push_back(temp);
}