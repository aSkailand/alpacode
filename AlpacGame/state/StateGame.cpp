
#include "StateGame.h"


void StateGame::goNext(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();

    // Clock used to throttle movement softly
    sf::Clock ticker;

    // Instantiating Entities
    // todo: throw in configWindow instead?
    Planet planet(*machine);
    Farmer farmer(*window);
    Alpaca alpaca(45,*machine);
    Alpaca alpaca1(90,*machine);
    Alpaca alpaca2(180,*machine);
    WolfState wolf;
    //Clock TODO: use the global clock
    sf::Clock clock;

    rotationSpeed = 100;

    // Check if game is ongoing

    // todo: Add common rotation calculation here?

    float delta = ticker.restart().asSeconds();

    while (pollGame()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            std::cout << "Right" << std::endl;
            rotationDelta = delta * rotationSpeed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            std::cout << "Left" << std::endl;
            rotationDelta = delta * -rotationSpeed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            std::cout << "Up" << std::endl;
            // todo: fix jump
            if (farmer.status != Farmer::Status::AIRBORNE) {
                farmer.status = Farmer::Status::AIRBORNE;
                farmer.velocity_y = -10;
                farmer.y += 50;
            }
        } else {
            rotationDelta = 0;
        }

        // Drawing Phase

        //Sends the elapsed time to the WolfState
        sf::Time elapsed = clock.getElapsedTime();
        wolf.goWolf((int) elapsed.asSeconds());
        if ((int) clock.getElapsedTime().asSeconds() == wolf.getTickSecond()) {
            clock.restart();
        }//END


        window->clear(sf::Color::Blue);

        planet.control(rotationDelta);
        farmer.control(rotationDelta);
        alpaca.control(45);
        alpaca1.control(90);
        alpaca2.control(180);
        planet.draw();
        farmer.draw();
        alpaca.draw();
        alpaca1.draw();
        alpaca2.draw();

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
