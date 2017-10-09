#include "SFML/Graphics.hpp"

#include "window.h"

window::window()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "AlpacGame");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);


        //draw here


        window.display();
    }


}
