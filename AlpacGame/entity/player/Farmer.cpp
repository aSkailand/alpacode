#include "Farmer.h"

Farmer::Farmer(StateMachine &stateMachine, float initAngle) {

    // Assigning pointers
    configGame = &stateMachine.configGame;
    window = &stateMachine.configWindow.getWindow();

    // Load textures
    loadTextures();

    // Assigning default states
    currentAction = Action::IDLE;
    currentStatus = Status::GROUNDED;
    currentDirection = Direction::RIGHT;

    // Define the alpaca
    farmer = sf::RectangleShape(sf::Vector2f(size, size));
    farmer.setTexture(&farmerTexture);
    farmer.setOrigin(farmer.getSize().x / 2, farmer.getSize().y);
    //farmer.setOutlineThickness(1);
    farmer.scale(-1.f, 1.f);

    // Assigning initial position
    angle = initAngle;
}

void Farmer::draw() {

    // Place the farmer accordingly
    farmer.setPosition(x,y);

    // Draw the square
    window->draw(farmer);
}

void Farmer::control() {

    // Move farmer according to input
    if(configGame->currentInput == sf::Keyboard::Right){
        angle += configGame->deltaTime*speed;
    }
    else if(configGame->currentInput == sf::Keyboard::Left){
        angle -= configGame->deltaTime*speed;
    }

    // todo: Simplify flipping?
    // Changes which way to flip the farmer
    if (configGame->currentInput == sf::Keyboard::Unknown) {
        currentAction = Action::IDLE;
    } else if (configGame->currentInput == sf::Keyboard::Right) {
        currentAction = Action::WALKING;
        if (currentDirection != Direction::RIGHT) {
            farmer.scale(-1.f, 1.f);
            currentDirection = Direction::RIGHT;
        }
    } else if (configGame->currentInput == sf::Keyboard::Left) {
        currentAction = Action::WALKING;
        if (currentDirection != Direction::LEFT) {
            farmer.scale(-1.f, 1.f);
            currentDirection = Direction::LEFT;
        }
    }

    // Update the position and rotation of farmer
    x = configGame->calcX(angle);
    y = configGame->calcY(angle);
    farmer.setRotation(angle);

}

void Farmer::loadTextures() {
    if (!farmerTexture.loadFromFile("entity/player/farmer.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}
