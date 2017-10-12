#include "planet.h"


void planetClass::control(float rotationDelta, StateGame::Direction rotationDir) {

    // Check if any of the arrow keys are pressed
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//        planet.rotate(rotationDelta);
//        alpaca.rotate(rotationDelta);
//        std::cout << planet.getRotation() << std::endl;
//    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//        planet.rotate(rotationDelta * (-1));
//        alpaca.rotate(rotationDelta * (-1));
//        std::cout << planet.getRotation() << std::endl;
//    }
//    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)){
//        std::cout << "Information: " << std::endl;
//        std::cout << "The radius is: " << planet.getRadius() << std::endl;
//        std::cout << "XPOS is: " << xpos << std::endl;
//        std::cout << "YPOS is: " << ypos << std::endl;
//    }
    if(rotationDir == StateGame::Direction::LEFT){
        planet.rotate(rotationDelta);
        alpaca.rotate(rotationDelta);
    } else{
        planet.rotate(-rotationDelta);
        alpaca.rotate(-rotationDelta);
    }

    // Calculate X and Y position
    xpos = windowSize.width / 2 + (radius * std::cos((planet.getRotation() * M_PI / 180)));
    ypos = windowSize.height + offset + (radius * (std::sin((planet.getRotation() * M_PI / 180))));

    alpaca.setPosition(xpos, ypos);

    if(currentDir != rotationDir){
        alpaca2.scale(-1.f,1.f);
        currentDir = rotationDir;
    }
}


void planetClass::draw() {
    window->draw(backgroundSprite);
    window->draw(planet);
    window->draw(alpaca2);
    window->draw(alpaca);
}

planetClass::planetClass(sf::RenderWindow &renderWindow) {

    window = &renderWindow;
    windowSize = sf::VideoMode(window->getSize().x, window->getSize().y);

    // Load Textures
    if (!planetTexture.loadFromFile("planet/wat.png")) {
        std::cout << "Error!!!" << std::endl;
    }


    if (!alpacaTexture.loadFromFile("planet/alpaca.png")) {
        std::cout << "Error!!!" << std::endl;
    }


    if (!backgroundTexture.loadFromFile("planet/back.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    // Define Background
    backgroundSprite = sf::Sprite(backgroundTexture);

    // Define planet
    planet = sf::CircleShape(radius);
    planet.setTexture(&planetTexture);
    planet.setOrigin(sf::Vector2f(radius, radius));
    planet.setPosition(windowSize.width / 2, windowSize.height + offset);

    // Define standing alpaca
    alpaca2 = sf::RectangleShape(sf::Vector2f(200, 200));
    alpaca2.setTexture(&alpacaTexture);
    alpaca2.setOrigin(alpaca2.getSize().x / 2, alpaca2.getSize().y);
    alpaca2.setPosition(windowSize.width / 2, windowSize.height + offset - radius);
    alpaca2.scale(-1.f,1.f);

    // Define moving alpaca
    alpaca = sf::RectangleShape(sf::Vector2f(200, 200));
    alpaca.setTexture(&alpacaTexture);
    alpaca.setOrigin(alpaca.getSize().x / 2, alpaca.getSize().y);
    alpaca.rotate(90);

}
