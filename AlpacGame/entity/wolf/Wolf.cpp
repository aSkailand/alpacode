#include "Wolf.h"

Wolf::Wolf(StateMachine &stateMachine, float initAngle) {

    // Assigning pointers
    configGame = &stateMachine.configGame;
    window = &stateMachine.configWindow.getWindow();

    // Load Textures
    loadTextures();

    // Assigning default states
    currentActionState = Action::IDLE;
    currentDirectionState = Direction::RIGHT;

    // Generate random number generator
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::default_random_engine(seed);

    // Define the wolf
    wolfRectangle = sf::RectangleShape(sf::Vector2f(size, size));
    wolfRectangle.setTexture(&wolfTexture);
    wolfRectangle.setOrigin(wolfRectangle.getSize().x / 2, wolfRectangle.getSize().y);

    // Assigning initial position
    angle = initAngle;
}

void Wolf::draw() {
    wolfRectangle.setRotation(angle);
    wolfRectangle.setPosition(x, y);
    window->draw(wolfRectangle);
}

void Wolf::control() {

    // Check if it is time for randomizing the wolf's current state
    if ((int) clock.getElapsedTime().asSeconds() >= tickSecond) {

        currentActionState = (Action) randomNumberGenerator(0, 1);

        switch (currentActionState) {
            case Wolf::Action::IDLE:{
                std::cout << " The wolf is idle..." << std::endl;
                break;
            }
            case Wolf::Action::WALKING: {
                currentDirectionState = (Direction) randomNumberGenerator(0, 1);
                switch (currentDirectionState) {
                    case Wolf::Direction::LEFT: {
                        std::cout << " The wolf is moving left!" << std::endl;
                        wolfRectangle.setScale({-1.f, 1.f});
                        break;
                    }
                    case Wolf::Direction::RIGHT: {
                        std::cout << " The wolf is moving right!" << std::endl;
                        wolfRectangle.setScale({1.f, 1.f});
                        break;
                    }
                }
                break;
            }
        }

        clock.restart();

    }

    // Reposition the wolf
    if (currentActionState == Action::WALKING) {
        if (currentDirectionState == Wolf::Direction::RIGHT) {
            angle += configGame->deltaTime * speed;
        } else if (currentDirectionState == Wolf::Direction::LEFT) {
            angle -= configGame->deltaTime * speed;

        }
    }

    // Position calculation
    x = configGame->calcX(angle);
    y = configGame->calcY(angle);

}

int Wolf::randomNumberGenerator(int lower, int upper) {
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}

void Wolf::loadTextures() {
    if (!wolfTexture.loadFromFile("entity/wolf/wolfy.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}
