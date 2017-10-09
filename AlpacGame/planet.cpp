#include "planet.h"


void planetClass::planet_controll() {

    int radius = 1000;
    int xpos = 0, ypos = 0;
    sf::RenderWindow alpacaGame(sf::VideoMode(3000, 1300), "Hello, World!");


    sf::CircleShape planet(radius);
    sf::Texture texture;


    sf::RectangleShape entity(sf::Vector2f(100, 100));
    entity.setSize(sf::Vector2f(100, 100));
    entity.setFillColor(sf::Color::Red);

    if (!texture.loadFromFile("earth.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    planet.setTexture(&texture);
    planet.setTextureRect(sf::IntRect(100, 100, 100, 100));
    planet.move(1280 / 2, 1300);
    planet.setOrigin(sf::Vector2f(1000, 1000));


    while (alpacaGame.isOpen()) {

        sf::Event event;

        while (alpacaGame.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                alpacaGame.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                planet.rotate(1);
                std::cout << planet.getRotation() << std::endl;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                planet.rotate(-1);
                std::cout << planet.getRotation() << std::endl;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
                std::cout << "Information: " << std::endl;
                std::cout << "The radius is: " << planet.getRadius() << std::endl;
                std::cout << "XPOS is: " << xpos << std::endl;
                std::cout << "YPOS is: " << ypos << std::endl;
            }

            //calculate x pos

            xpos = (int) (radius * (std::cos((planet.getRotation() * M_PI / 180))));
            ypos = (int) (radius * (std::sin((planet.getRotation() * M_PI / 180))));


            entity.setPosition(xpos, ypos);


            alpacaGame.clear(sf::Color::Cyan);

            alpacaGame.draw(planet);
            alpacaGame.draw(entity);
            alpacaGame.display();
        }
    }
}