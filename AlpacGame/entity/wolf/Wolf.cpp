#include "Wolf.h"

Wolf::Wolf(StateMachine &stateMachine, float initAngle) : id(nextId++){

    // Assigning pointers
    configGame = &stateMachine.configGame;
    window = &stateMachine.configWindow.getWindow();

    // Load textures
    loadTextures();

    // Assigning default states
    currentAction = Action::IDLE;
    currentDirection = Direction::RIGHT;

    // Generate random number generator
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count() + id;
    generator = std::default_random_engine(seed);

    // Define the wolf
    wolfshape = sf::RectangleShape(sf::Vector2f(size, size));
    wolfshape.setTexture(&wolfTexture);
    wolfshape.setOrigin(wolfshape.getSize().x / 2, wolfshape.getSize().y);
    wolfshape.setOutlineThickness(1);


    // Assigning initial position
    angle = initAngle;
}

int Wolf::nextId = 0;

void Wolf::draw() {

    /* The reason why setPosition and setRotation is here and not in control() is
     * because when adding movement to wolves, the x and y change one more time (First
     * time being the reposition because of the planet's rotation).
     * Therefore we don't want setPosition and setRotation in control(), where
     * this movement will be handled. */

    // Place the wolf accordingly
    wolfshape.setRotation(angle);
    wolfshape.setPosition(x, y);

    // Draw the square
    window->draw(wolfshape);
}

void Wolf::control() {

    // Check if it is time for randomizing the wolf's current state
    if ((int) clock.getElapsedTime().asSeconds() >= tickSecond) {

        currentAction = (Action) randomNumberGenerator(0, 1);

        switch (currentAction) {
            case Wolf::Action::IDLE:{
                std::cout << "Wolf " << id << " is now IDLE." << std::endl;
                break;
            }
            case Wolf::Action::WALKING: {
                currentDirection = (Direction) randomNumberGenerator(0, 1);
                switch (currentDirection) {
                    case Wolf::Direction::LEFT: {
                        std::cout << "Wolf " << id << " is now WALKING LEFT" << std::endl;
                        wolfshape.setScale({-1.f, 1.f});
                        break;
                    }
                    case Wolf::Direction::RIGHT: {
                        std::cout << "Wolf " << id << " is now WALKING RIGHT" << std::endl;
                        wolfshape.setScale({1.f, 1.f});
                        break;
                    }
                }
                break;
            }
        }

        clock.restart();

    }

    // Reposition the wolf
    if (currentAction == Action::WALKING) {
        if (currentDirection == Wolf::Direction::RIGHT) {
            angle += configGame->deltaTime * speed;
        } else if (currentDirection == Wolf::Direction::LEFT) {
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
