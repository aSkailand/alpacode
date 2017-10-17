
#include "Farmer.h"

Farmer::Farmer(StateMachine &stateMachine, float initAngle) {

    // Assigning pointers
    window = &stateMachine.configWindow.getWindow();
    configGame = &stateMachine.configGame;

    // Loading textures
    loadTextures();

    // Create Farmer
    farmer = sf::RectangleShape(sf::Vector2f(size, size));
    farmer.setTexture(&farmerTexture);
    farmer.setOrigin(farmer.getSize().x / 2, farmer.getSize().y);
    farmer.setOutlineThickness(1);
    farmer.scale(-1.f, 1.f);

    // Assigning enums
    action = Action::IDLE;
    status = Status::GROUNDED;
    direction = Direction::RIGHT;

    // Assigning initial position
    angle = initAngle;

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
        action = Action::IDLE;
    } else if (configGame->currentInput == sf::Keyboard::Right) {
        action = Action::WALKING;
        if (direction != Direction::RIGHT) {
            farmer.scale(-1.f, 1.f);
            direction = Direction::RIGHT;
        }
    } else if (configGame->currentInput == sf::Keyboard::Left) {
        action = Action::WALKING;
        if (direction != Direction::LEFT) {
            farmer.scale(-1.f, 1.f);
            direction = Direction::LEFT;
        }
    }

    // Update the position and rotation of farmer
    x = configGame->calcX(angle);
    y = configGame->calcY(angle);
    farmer.setRotation(angle);

}

void Farmer::draw() {

    // Update position
    farmer.setPosition(x,y);

    // Draw the square
    window->draw(farmer);
}

void Farmer::loadTextures() {
    if (!farmerTexture.loadFromFile("entity/player/farmer.png")) {
        std::cout << "Error!!!" << std::endl;
    }
}
