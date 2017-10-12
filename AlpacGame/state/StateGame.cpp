
#include "StateGame.h"
#include "../planet/planet.h"

void StateGame::goNext(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->config.getWindow();

    // Clock used to throttle movement softly
    sf::Clock ticker;

    // Instantiating Entities
    planetClass planet(*window);    // todo: throw in config instead?

    rotationSpeed = 100;

    // Check if game is ongoing
    while(pollGame()){

        // todo: Add common rotation calculation here?

        float delta = ticker.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            rotationDelta = delta * rotationSpeed;
            rotationDir = Direction::RIGHT;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            rotationDelta = delta * rotationSpeed;
            rotationDir = Direction::LEFT;
        } else{
            rotationDelta = 0;
        }

        // Drawing Phase
        window->clear(sf::Color::Blue);

        planet.control(rotationDelta, rotationDir);
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
