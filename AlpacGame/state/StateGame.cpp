
#include "StateGame.h"

void StateGame::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->config.getWindow();

    Alpaca Alpa(300, 200, config, *window);
    int times = 1;


    while(pollGame()){
        currentTime = clock.getElapsedTime();
        int cT = (int)clock.getElapsedTime().asSeconds();


        if (cT == times){
            std::cout << "Seconds = " << times << std::endl;

            if (times % 3 == 0){
                Alpa.randomAction();
            }

            times++;
        }


        Alpa.move(0.001);
        window->clear(sf::Color::Blue);
        Alpa.draw();

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
