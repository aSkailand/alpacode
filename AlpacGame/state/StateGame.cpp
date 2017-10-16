
#include "StateGame.h"


void StateGame::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->config.getWindow();
    window->setFramerateLimit(60);


    //Wolf
    WolfState wolf;
    //Clock TODO: use the global clock
    sf::Clock clock;


    while (pollGame()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            std::cout << "Right" << std::endl;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            std::cout << "Left" << std::endl;
        }

        //Sends the elapsed time to the WolfState
        sf::Time elapsed = clock.getElapsedTime();
        wolf.goWolf((int)elapsed.asSeconds());
        if((int)clock.getElapsedTime().asSeconds() == wolf.getTickSecond()){
            clock.restart();
        }//END




        window->clear(sf::Color::Blue);
        window->display();
    }
}

bool StateGame::pollGame() {
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                machine->setCurrentState(StateMachine::stateID::MENU);
                return false;
            }
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::Escape) {
                    machine->setCurrentState(StateMachine::stateID::MENU);
                    return false;
                } else if (event.key.code == sf::Keyboard::Space) {
                    // todo: Add actual pause.
                    machine->setCurrentState(StateMachine::stateID::PAUSE);
                    return false;
                }
            }
            default: {
                return true;
            }
        }
    }
    return true;
}
