
#include "Farmer.h"

Farmer::Farmer(sf::RenderWindow &renderWindow) {

    // Assigning window
    window = &renderWindow;
    windowSize = sf::VideoMode(window->getSize().x, window->getSize().y);

    // Load texture
    if (!farmerTexture.loadFromFile("entity/player/farmer.png")) { // todo: change path
        std::cout << "Error!!!" << std::endl;
    }

    // Assigning variables
    y = windowSize.height / 2 - 100;    // todo: -100 is magic number, change it to relative!
    velocity_y = 0;
    action = Action::IDLE;
    status = Status::GROUNDED;
    direction = Direction::RIGHT;

    // Create Farmer
    rectangle = sf::RectangleShape(sf::Vector2f(size, size));
    rectangle.setTexture(&farmerTexture);
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y);
    rectangle.setPosition(windowSize.width / 2, y);
    rectangle.setOutlineThickness(1);

}

void Farmer::draw() {
    // Draw the square
    window->draw(rectangle);
}

void Farmer::control(float rotationDelta) {

    // Gravity Control
    if (y < windowSize.height / 2 - 100){
        velocity_y += gravity;
    }
    else if (y>windowSize.height / 2 - 100){
        y = windowSize.height / 2 - 100;
        status = Farmer::Status::GROUNDED;
    }

    y += velocity_y;

    rectangle.setPosition(rectangle.getPosition().x,y);

    // Changes which way to flip the farmer
    if (rotationDelta == 0) {
        action = Action::IDLE;
    } else if (rotationDelta < 0) {
        action = Action::WALKING;
        if (direction != Direction::RIGHT) {
            rectangle.scale(-1.f, 1.f);
            direction = Direction::RIGHT;
        }
    } else if (rotationDelta > 0) {
        action = Action::WALKING;
        if (direction != Direction::LEFT) {
            rectangle.scale(-1.f, 1.f);
            direction = Direction::LEFT;
        }
    }
}
