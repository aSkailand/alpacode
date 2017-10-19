
#include "planet.h"

Planet::Planet(StateMachine &stateMachine) {

    // Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();

    // Load textures
    loadTextures();

    // Define Background
    backgroundSprite = sf::Sprite(backgroundTexture);

    // Creating Planet
    float radius = stateMachine.configGame.planetRadius;
    planet = sf::CircleShape(radius);
    planet.setTexture(&planetTexture);
    planet.setOrigin(sf::Vector2f(radius, radius));
    planet.setPosition(stateMachine.configGame.planetCenter);

}

void Planet::draw() {
    window->draw(backgroundSprite);
    window->draw(planet);
}

void Planet::loadTextures() {

    // Load Textures
    if (!planetTexture.loadFromFile("planet/planet.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("planet/back.png")) {
        std::cout << "Error!!!" << std::endl;
    }
}
