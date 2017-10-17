
#include "StateGame.h"

void StateGame::goNext(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    configGame = &machine->configGame;

    // Clock used to throttle movement softly
    sf::Clock ticker;

    // todo: *StateMachine too much permission? Perhaps a combo of *configGame and *configWindow?
    // todo: Add to vector in the future, makes handling entities tenfold simpler.
    // Instantiating Entities
    Planet planet(*machine);

    Farmer farmer(*machine, 0);

    Alpaca alpaca1(*machine, 90);
    Alpaca alpaca2(*machine, 180);
    Alpaca alpaca3(*machine, 190);
    Alpaca alpaca4(*machine, 273);
    Alpaca alpaca5(*machine, 23);
    Alpaca alpaca6(*machine, 65);

    WolfState wolf;

    //Clock TODO: use the global clock
    sf::Clock clock;

    // Check if game is ongoing
    while (pollGame()) {

        configGame->deltaTime = ticker.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            configGame->currentInput = sf::Keyboard::Right;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            configGame->currentInput = sf::Keyboard::Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            configGame->currentInput = sf::Keyboard::Up;
        } else {
            configGame->currentInput = sf::Keyboard::Unknown;
        }


        // Todo: Delete when wolf is implemented correctly
        //Sends the elapsed time to the WolfState
        sf::Time elapsed = clock.getElapsedTime();
        wolf.goWolf((int) elapsed.asSeconds());
        if ((int) clock.getElapsedTime().asSeconds() == wolf.getTickSecond()) {
            clock.restart();
        }//END


        // todo: Create a drawing function?
        // Drawing Phase

        window->clear(sf::Color::Blue);

        farmer.control();

        alpaca1.control();
        alpaca2.control();
        alpaca3.control();
        alpaca4.control();
        alpaca5.control();
        alpaca6.control();

        planet.draw();

        farmer.draw();

        alpaca1.draw();
        alpaca2.draw();
        alpaca3.draw();
        alpaca4.draw();
        alpaca5.draw();
        alpaca6.draw();

        window->display();

        // todo: Create a function for view?
        // Update View
        view = sf::View(window->getDefaultView());
        view.zoom(viewZoom);
        view.setCenter(configGame->calcX(farmer.angle, viewOffset), configGame->calcY(farmer.angle, viewOffset));
        view.setRotation(farmer.farmer.getRotation());
        window->setView(view);
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
//                    machine->setCurrentState(StateMachine::stateID::PAUSE);
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
