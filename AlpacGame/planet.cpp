#include "planet.h"


void planetClass::planet_control() {

    // loads texture
    sf::Texture texture;
    if (!texture.loadFromFile("wat.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    sf::Texture alpaca;
    if (!alpaca.loadFromFile("alpaca.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    sf::Texture background;
    if (!background.loadFromFile("back.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    background.setSmooth(true);

    sf::Sprite back(background);

    sf::RenderWindow alpacaGame(windowSize, "Alpaca Worlds!");
    alpacaGame.setFramerateLimit(60);

    // Define planet
    sf::CircleShape planet(radius);
    planet.setTexture(&texture);
    planet.setOrigin(sf::Vector2f(radius, radius));
    planet.setPosition(windowSize.width/2, windowSize.height+offset);

    // Define entity
    sf::RectangleShape entity(sf::Vector2f(200,200));
    entity.setTexture(&alpaca);
    entity.setOrigin(entity.getSize().x/2, entity.getSize().y);
    entity.rotate(90);

    // Clock used to throttle movement softly
    sf::Clock ticker;

    while (alpacaGame.isOpen()) {

        // Restart timer for each iteration
        float delta = ticker.restart().asSeconds();

        sf::Event event;
        while (alpacaGame.pollEvent(event)) {
            switch (event.type){
                case sf::Event::Closed:{
                    alpacaGame.close();
                    break;
                }
                case sf::Event::KeyPressed:{
                    if(event.key.code == sf::Keyboard::Escape){
                        alpacaGame.close();
                        break;
                    }
                    else if(event.key.code == sf::Keyboard::I){
                        std::cout << "Information: " << std::endl;
                        std::cout << "The radius is: " << planet.getRadius() << std::endl;
                        std::cout << "XPOS is: " << xpos << std::endl;
                        std::cout << "YPOS is: " << ypos << std::endl;
                    }
                }
            }
        }


        // Check if any of the arrow keys are pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            planet.rotate(rotationSpeed*delta);
            entity.rotate(rotationSpeed*delta);
            std::cout << planet.getRotation() << std::endl;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            planet.rotate(rotationSpeed*delta*(-1));
            entity.rotate(rotationSpeed*delta*(-1));
            std::cout << planet.getRotation() << std::endl;
        }

        // Calculate X and Y position
        xpos = windowSize.width/2 + (radius * std::cos((planet.getRotation() * M_PI / 180)));
        ypos = windowSize.height + offset + (radius* (std::sin((planet.getRotation() * M_PI / 180))));

        entity.setPosition(xpos, ypos);

        // Draw
        alpacaGame.clear(sf::Color::Cyan);

        alpacaGame.draw(back);
        alpacaGame.draw(planet);
        alpacaGame.draw(entity);

        alpacaGame.display();
    }
}