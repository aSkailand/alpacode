
#include "StateMenu.h"

void StateMenu::goNext(StateMachine &stateMachine) {

    initMenuStrings();

    sf::RenderWindow &window = stateMachine.config.getWindow();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed : {
                    switch (event.key.code) {
                        case sf::Keyboard::Up : {
                            moveUp();
                            break;
                        }
                        case sf::Keyboard::Down : {
                            moveDown();
                            break;
                        }
                        case sf::Keyboard::Return : {
                            switch (GetPressedItem()) {
                                case 0 : {
                                    std::cout << "Play" << std::endl;
                                    break;
                                }

                                case 1 : {
                                    std::cout << "Options" << std::endl;
                                    break;
                                }

                                case 2 : {
                                    stateMachine.setCurrentState(StateMachine::stateID::EXIT);
                                    window.close();
                                }

                            }
                        }
                    }
                    break;
                }
            }


            if (event.type == sf::Event::Closed) {
                stateMachine.setCurrentState(StateMachine::stateID::EXIT);
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        window.draw(menu[0]);
        window.draw(menu[1]);
        window.draw(menu[2]);

        window.display();
    }

    stateMachine.setCurrentState(StateMachine::stateID::EXIT);


}

void StateMenu::initMenuStrings() {
    font.loadFromFile("font.ttf");

    menu[0].setFont(font);
    menu[0].setString("Play");
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setPosition(350, 150);

    menu[1].setFont(font);
    menu[1].setString("Option");
    menu[1].setFillColor(sf::Color::Green);
    menu[1].setPosition(350, 250);

    menu[2].setFont(font);
    menu[2].setString("Exit");
    menu[2].setFillColor(sf::Color::Green);
    menu[2].setPosition(350, 350);

}

void StateMenu::moveUp() {
    if (menuIndex - 1 >= 0) {
        menu[menuIndex].setFillColor(sf::Color::Green);
        menuIndex--;
        menu[menuIndex].setFillColor(sf::Color::Red);
    }

}

void StateMenu::moveDown() {
    if (menuIndex + 1 <= 2) {
        menu[menuIndex].setFillColor(sf::Color::Green);
        menuIndex++;
        menu[menuIndex].setFillColor(sf::Color::Red);
    }

}
