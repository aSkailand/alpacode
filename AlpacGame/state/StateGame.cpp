
#include "StateGame.h"


void StateGame::goNext(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();

    // Clock used to throttle movement softly
    sf::Clock ticker;

    // todo: *StateMachine too much permission? Perhaps a combo of *configGame and *configWindow?
    // todo: Add to vector in the future, makes handling entities tenfold simpler.
    // Instantiating Entities
    Planet planet(*machine);

    Farmer farmer(*window);

    Alpaca alpaca0(*machine, 0);
    Alpaca alpaca1(*machine, 90);
    Alpaca alpaca2(*machine, 180);
    Alpaca alpaca3(*machine, 190);
    Alpaca alpaca4(*machine, 273);
    Alpaca alpaca5(*machine, 23);
    Alpaca alpaca6(*machine, 65);

    WolfState wolf;

    //Clock TODO: use the global clock
    sf::Clock clock;

    float delta = ticker.restart().asSeconds();

    // Check if game is ongoing
    while (pollGame()) {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            std::cout << "Right" << std::endl;
            rotationDelta = delta * rotationSpeed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            std::cout << "Left" << std::endl;
            rotationDelta = delta * rotationSpeed * (-1);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            std::cout << "Up" << std::endl;
            // todo: fix jump, and add it to farmer and not here
            if (farmer.status != Farmer::Status::AIRBORNE) {
                farmer.status = Farmer::Status::AIRBORNE;
                farmer.velocity_y = -10;
                farmer.y += 50;
            }
        } else {
            rotationDelta = 0;
        }


        //Sends the elapsed time to the WolfState
        sf::Time elapsed = clock.getElapsedTime();
        wolf.goWolf((int) elapsed.asSeconds());
        if ((int) clock.getElapsedTime().asSeconds() == wolf.getTickSecond()) {
            clock.restart();
        }//END


        // todo: Create a drawing function
        // Drawing Phase

        window->clear(sf::Color::Blue);

        planet.control(rotationDelta);

        farmer.control(rotationDelta);

        alpaca0.control();
        alpaca1.control();
        alpaca2.control();
        alpaca3.control();
        alpaca4.control();
        alpaca5.control();
        alpaca6.control();

        planet.draw();
        farmer.draw();
        alpaca0.draw();
        alpaca1.draw();
        alpaca2.draw();
        alpaca3.draw();
        alpaca4.draw();
        alpaca5.draw();
        alpaca6.draw();

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
