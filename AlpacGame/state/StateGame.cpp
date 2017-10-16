
#include "StateGame.h"


void StateGame::goNext(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->config.getWindow();
    window->setFramerateLimit(60);


    //Wolf
    Wolf wolf(*window);

    //Clock TODO: use the global clock
    sf::Clock clock;



    // Clock used to throttle movement softly
    sf::Clock ticker;

    // Instantiating Entities
    // todo: throw in config instead?
    Planet planet(*window);
    Farmer farmer(*window);

    rotationSpeed = 100;

    // Check if game is ongoing


        // todo: Add common rotation calculation here?

        float delta = ticker.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            std::cout << "Right" << std::endl;
            rotationDelta = delta * rotationSpeed;
        }

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





            window->clear(sf::Color::Blue);

            planet.control(rotationDelta);
            farmer.control(rotationDelta);

            wolf.control(rotationDelta); //TODO:: get rotation to work controll the aplaca


            planet.draw();
            farmer.draw();
            wolf.draw();

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
