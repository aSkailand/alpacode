
#include "planet.h"

Planet::Planet(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    windowSize = sf::VideoMode(window->getSize().x, window->getSize().y);

    // Load textures
    loadTextures();

    // Define Background
    backgroundSprite = sf::Sprite(backgroundTexture);

    // Define planet
    radius = stateMachine.configGame.planetRadius;
    offset = stateMachine.configGame.offset; // todo: remove in future updates?

    planet = sf::CircleShape(radius);
    planet.setTexture(&planetTexture);
    planet.setOrigin(sf::Vector2f(radius, radius));
    planet.setPosition(stateMachine.configGame.planetCenter);

//    // Define moving alpaca
//    alpaca = sf::RectangleShape(sf::Vector2f(150, 150));
//    alpaca.setTexture(&alpacaTexture);
//    alpaca.setOrigin(alpaca.getSize().x / 2, alpaca.getSize().y);
//    alpaca.rotate(90);
//    alpaca.setOutlineThickness(1);

}


void Planet::control(float rotationDelta) {

    // todo: Delete this debug check?
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)){
        std::cout << "Information: " << std::endl;
        std::cout << "The radius is: " << planet.getRadius() << std::endl;
        std::cout << "XPOS is: " << xpos << std::endl;
        std::cout << "YPOS is: " << ypos << std::endl;
    }

    planet.rotate(-rotationDelta);
    machine->configGame.planetRotation = planet.getRotation();

//    alpaca.rotate(-rotationDelta);

    // Calculate X and Y position
    xpos = windowSize.width / 2 + (radius * std::cos((planet.getRotation() * M_PI / 180)));
    ypos = windowSize.height + offset + (radius * (std::sin((planet.getRotation() * M_PI / 180))));

//    alpaca.setPosition(xpos, ypos);

}

void Planet::draw() {
    window->draw(backgroundSprite);
    window->draw(planet);
//    window->draw(alpaca);
}

void Planet::loadTextures() {

    // Load Textures
    if (!planetTexture.loadFromFile("planet/wat.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("planet/back.png")) {
        std::cout << "Error!!!" << std::endl;
    }
}
