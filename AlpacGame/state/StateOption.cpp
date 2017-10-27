#include "StateOption.h"

void StateOption::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->configWindow.getWindow();

    initOptionStrings();

    while (PollOption()){
        drawOption();
    }

}

bool StateOption::PollOption() {
    sf::Event event;
    while (window->pollEvent(event)){
        switch (event.type){
            case sf::Event::Closed : {
                machine->setCurrentState(StateMachine::stateID::EXIT);
                return false;
            }
            case sf::Event::KeyPressed : {
                if (event.key.code == sf::Keyboard::Escape) {
                    machine->setCurrentState(StateMachine::stateID::MENU);
                    return false;
                }
                switch (event.key.code){
                    case sf::Keyboard::Up : {
                        moveUp();
                        return true;
                    }
                    case sf::Keyboard::Down : {
                        moveDown();
                        return true;
                    }
                    case sf::Keyboard::Return : {
                        switch (optionIndex) {
                            case 0 : {
                                /**
                                 * Sets resolution to 800x600
                                 */
                                lowRes();
                                return true;
                            }
                            case 1 : {
                                /**
                                 * Sets resolution to 1280x960
                                 */
                                highRes();
                                return true;
                            }
                            case 2 : {
                                /**
                                 * Goes back to menu
                                 */
                                machine->setCurrentState(StateMachine::stateID::MENU);
                                return false;
                            }
                            default : {
                                std::cout << "Error: ran default in stateOption.cpp keyboard::return" << std::endl;
                                break;
                        }
                        }
                    }
                    default : {
                        std::cout << "Error: switch(event.key.code)" << std::endl;
                        break;
                    }
                }
                break;
            }
        }
    }
    return true;
}

void StateOption::drawOption() {
    window->clear(sf::Color::Black);

    for (const sf::Text &text : optionChoices) {
        window->draw(text);
    }

    window->display();
}

void StateOption::initOptionStrings() {
    fontOptionChoice.loadFromFile("Resources/fontMenu.ttf");

    optionChoices.clear();

    addOptionChoice("Low Resolution", 250, 150); //800x600
    addOptionChoice("Medium Resolution", 250, 250); //1280x960
    addOptionChoice("Back", 50, 500);

    optionIndex = 0;
    optionChoices[0].setFillColor(colorSelected);
}

void StateOption::addOptionChoice(const std::string &choice, int x, int y) {
    sf::Text temp = sf::Text(choice, fontOptionChoice, fontSizeOptionChoice);
    temp.setPosition(x, y);
    temp.setFillColor(colorDeselected);
    optionChoices.push_back(temp);
}

void StateOption::moveUp() {
    if (optionIndex - 1 >= 0) {
        optionChoices[optionIndex].setFillColor(colorDeselected);
        optionIndex--;
        optionChoices[optionIndex].setFillColor(colorSelected);
    }
}

void StateOption::moveDown() {
    if (optionIndex + 1 < optionChoices.size()) {
        optionChoices[optionIndex].setFillColor(colorDeselected);
        optionIndex++;
        optionChoices[optionIndex].setFillColor(colorSelected);
    }
}

void StateOption::lowRes() {
    machine->configWindow.setWindowResolution(800, 600);
}

void StateOption::highRes() {
    machine->configWindow.setWindowResolution(1280, 960);
}