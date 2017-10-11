
#include "StateGame.h"
#include "../planet/planet.h"

void StateGame::goNext(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->config.getWindow();

    // Clock used to throttle movement softly
    sf::Clock ticker;

    // Instantiating Entities
    planetClass planet(*window);    // todo: throw in config instead

    // Check if game is ongoing
    while(pollGame()){

        float delta = ticker.restart().asSeconds();

        // Drawing Phase
        window->clear(sf::Color::Blue);

        planet.control(delta);
        planet.draw();

        window->display();
    }
}

bool StateGame::pollGame() {
    sf::Event event;
    while (window->pollEvent(event)){
        switch (event.type){
            case sf::Event::Closed:{
                machine->setCurrentState(StateMachine::stateID::MENU);
                return false;
            }
            case sf::Event::KeyPressed:{
                if(event.key.code == sf::Keyboard::Escape){
                    machine->setCurrentState(StateMachine::stateID::MENU);
                    return false;
                } else if(event.key.code == sf::Keyboard::Space){
                    // todo: Add actual pause.
                    machine->setCurrentState(StateMachine::stateID::PAUSE);
                    return false;
                }
            }
            default:{
                return true;
            }
        }
    }
    return true;
}
