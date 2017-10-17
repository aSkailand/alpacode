
#include "Alpaca.h"

Alpaca::Alpaca(StateMachine &stateMachine, float initAngle) {

    // Assign pointers
    window = &stateMachine.configWindow.getWindow();
    configGame = &stateMachine.configGame;

    // Load textures
    loadTextures();

    // Define the alpaca
    alpaca = sf::RectangleShape(sf::Vector2f(size, size));
    alpaca.setTexture(&alpacaTexture);
    alpaca.setOrigin(alpaca.getSize().x / 2, alpaca.getSize().y);
    alpaca.setOutlineThickness(1);

    // Set innate angle
    angle = initAngle;

    // Change enums
    currentAction = Action::IDLE;
    currentDirection = Direction::RIGHT;

}

void Alpaca::control() {

    // Handle alpaca's position relatively to planet's rotation
    x = configGame->calcX(configGame->planetRotation + angle);
    y = configGame->calcY(configGame->planetRotation + angle);


    // Handle alpaca's position according to its innate movements
    if (actionTick < clock.getElapsedTime().asSeconds()) {
        randomAction();
        clock.restart();
    }
}

void Alpaca::draw() {

    /* The reason why setPosition and setRotation is here and not in control() is
     * because when adding movement to alpacas, the x and y change one more time (First
     * time being the reposition because of the planet's rotation).
     * Therefore we don't want setPosition and setRotation in control(), where
     * this movement will be handled. */

    // Place the alpaca accordingly
    alpaca.setRotation(configGame->planetRotation + angle);
    alpaca.setPosition(x, y);

    // Draw the square
    window->draw(alpaca);
}


void Alpaca::randomAction() {

    // todo: Combine common random number generator
    // Random number generator
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);
    int random = distribution(generator);

    currentAction = (Action) random;

    switch (currentAction) {
        case Action::IDLE: {
            std::cout << "Alpaca is now IDLE." << std::endl;
            break;
        }
        case Action::WALKING: {
            std::cout << "Alpaca is now WALKING ";
            currentDirection = (Direction) distribution(generator);
            randomDirection();
        }
    }
}

void Alpaca::randomDirection() {
    switch (currentDirection) {
        case Direction::RIGHT: {
            std::cout << "RIGHT." << std::endl;
            break;
        }
        case Direction::LEFT: {
            std::cout << "LEFT." << std::endl;
            break;
        }
    }
}

void Alpaca::loadTextures() {
    if (!alpacaTexture.loadFromFile("entity/alpaca/alpaca.png")) {
        std::cout << "Error!!!" << std::endl;
    }
}


