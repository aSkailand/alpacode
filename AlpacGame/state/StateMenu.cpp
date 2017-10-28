#include "StateMenu.h"



void StateMenu::goNext(StateMachine &stateMachine) {



    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    window->setView(sf::View(window->getDefaultView()));
    menuGUI = &machine->configWindow.getMenuGUI();


    // GUI STUFF TODO: CLEAN THIS UP!

    // Creates a theme

    tgui::Theme::Ptr theme = tgui::Theme::create("C:/dev/libs/TGUI/include/TGUI/widgets/Black.txt");

    // Creates a layout
    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    layout->setSize(window->getSize().x / 2, window->getSize().y / 2);
    layout->setPosition(window->getSize().x / 4, window->getSize().y / 4);


    // Creates a button
    tgui::Button::Ptr button = theme->load("Button");
    button->setText("Play Game!");
    button->setSize(100, 20);

    tgui::Button::Ptr button1 = tgui::Button::copy(button);
    button1->setText("Settings");

    tgui::Button::Ptr button2 = tgui::Button::copy(button);
    button2->setText("Quit");



    button1->connect("pressed", PPrint);



    layout->add(button);
    layout->addSpace();
    layout->add(button1);
    layout->addSpace();
    layout->add(button2);
    layout->addSpace();
    menuGUI->add(layout);




    //initMenuStrings();

    while (pollMenu()) {
        drawMenu();


    }
}

bool StateMenu::pollMenu() {
    sf::Event event;
    while (window->pollEvent(event)) {
        menuGUI->handleEvent(event);
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




    }
    return true;
}

void StateMenu::drawMenu() {



    window->clear(sf::Color::Black);



    /*for (const sf::Text &text : menuChoices) {
        window->draw(text);
    }*/
    menuGUI->draw();
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

void StateMenu::OptionsState() {
    machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER);
}
