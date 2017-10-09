#include "planet.h"


void planetClass::planet_controll() {
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



    int radius = 1000;
    int xpos = 0, ypos = 0;

    sf::RenderWindow alpacaGame(sf::VideoMode(800, 600), "Alpaca Worlds!");

    sf::CircleShape planet(radius);
    planet.setTexture(&texture);

    planet.move(1280 / 2, 1300);
    planet.setOrigin(sf::Vector2f(1000, 1000));


    sf::RectangleShape entity(sf::Vector2f(0,0 ));
    entity.setSize(sf::Vector2f(200, 200));
    entity.setTexture(&alpaca);
    entity.rotate(90);

    while (alpacaGame.isOpen()) {

        sf::Event event;
        while (alpacaGame.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                alpacaGame.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                planet.rotate(1);
                entity.rotate(1);
                std::cout << planet.getRotation() << std::endl;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                planet.rotate(-1);
                entity.rotate(-1);
                std::cout << planet.getRotation() << std::endl;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
                std::cout << "Information: " << std::endl;
                std::cout << "The radius is: " << planet.getRadius() << std::endl;
                std::cout << "XPOS is: " << xpos << std::endl;
                std::cout << "YPOS is: " << ypos << std::endl;
            }

            //calculate X and Y position
            xpos = (int) 1280 / 2 + ((radius+150) * (std::cos((planet.getRotation() * M_PI / 180))));
            ypos = (int) 1300 + ((radius+150) * (std::sin((planet.getRotation() * M_PI / 180))));


            entity.setPosition(xpos, ypos);

            alpacaGame.clear(sf::Color::Cyan);

            alpacaGame.draw(back);

            alpacaGame.draw(planet);
            alpacaGame.draw(entity);
            alpacaGame.display();
        }
    }
}