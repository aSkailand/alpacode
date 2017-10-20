
#include "Alpaca.h"

Alpaca::Alpaca(StateMachine &stateMachine, float initAngle) {

    // Assign pointers
    window = &stateMachine.configWindow.getWindow();
    configGame = &stateMachine.configGame;

    // Create first alpaca id


    // Load textures
    loadTextures();

    // Define the alpaca
    alpaca = sf::RectangleShape(sf::Vector2f(size, size));
    alpaca.setTexture(&alpacaTexture);
    alpaca.setOrigin(alpaca.getSize().x / 2, alpaca.getSize().y);
    alpaca.setOutlineThickness(1);
    alpaca.setScale({-1.f, 1.f});


    // Set innate angle
    angle = initAngle;

    // Change enums
    currentAction = Action::IDLE;
    currentDirection = Direction::RIGHT;

}

void Alpaca::control() {

    // Handle walking
    if (currentAction == Action::WALKING) {
        if (currentDirection == Direction::RIGHT) {
            angle += configGame->deltaTime * speed;
        } else {
            angle -= configGame->deltaTime * speed;
        }
    }

    // Handle alpaca's position relatively to planet's rotation
    x = configGame->calcX(angle);
    y = configGame->calcY(angle);

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
    alpaca.setRotation(angle);
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
            std::cout << "Alpaca " << "is now IDLE." << std::endl;
            break;
        }
        case Action::WALKING: {
            std::cout << "Alpaca " << "is now WALKING " << std::endl;
            currentDirection = (Direction) distribution(generator);
            randomDirection();
        }
    }
}

void Alpaca::randomDirection() {
    switch (currentDirection) {
        case Direction::RIGHT: {
            std::cout << "RIGHT." << std::endl;
            alpacaTexture.loadFromFile("entity/alpaca/alpaca.png");
            break;
        }
        case Direction::LEFT: {
            std::cout << "LEFT." << std::endl;
            alpaca.setScale({1.f, 1.f});
            alpacaTexture.loadFromFile("entity/alpaca/alpaca.png");
            break;
        }
    }
}


void Alpaca::loadTextures() {

    if (!alpacaTexture.loadFromFile("entity/alpaca/alpaca.png")) {
        std::cout << "Error!!!" << std::endl;
    }

}


