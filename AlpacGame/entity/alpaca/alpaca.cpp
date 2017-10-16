
#include "alpaca.h"

Alpaca::Alpaca(float initDegree, StateMachine &stateMachine) {

    // Assign window
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();

    // Load textures
    if (!alpacaTexture.loadFromFile("entity/alpaca/alpaca.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    // Define the alpaca
    alpaca = sf::RectangleShape(sf::Vector2f(150, 150));
    alpaca.setTexture(&alpacaTexture);
    alpaca.setOrigin(alpaca.getSize().x / 2, alpaca.getSize().y);
    alpaca.rotate(90);
    alpaca.setOutlineThickness(1);

    x = machine->configGame.calcX(initDegree);
    y = machine->configGame.calcY(initDegree);


    // Change enums
    currentAction = Action::IDLE;
    currentDirection = Direction::RIGHT;

}

void Alpaca::control(float deltaRotation) {

    x = machine->configGame.calcX(-machine->configGame.planetRotation);
    y = machine->configGame.calcY(-machine->configGame.planetRotation);
    alpaca.rotate(machine->configGame.planetRotation);

    if (actionTick < clock.getElapsedTime().asSeconds()) {
        randomAction();
        clock.restart();
    }
}

void Alpaca::draw() {


    // Draw the square
    alpaca.setPosition(x,y);
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


