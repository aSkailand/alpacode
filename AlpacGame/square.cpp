#include <SFML/Graphics.hpp>
#include <iostream>

#include "square.h"

Square::Square(float x, float y, const Config& config, sf::RenderWindow& window)
        : x(x), y(y),
          config(config),
          window(window)

{
    rectangle.setSize(sf::Vector2f(size, size));
    rectangle.setFillColor(sf::Color::Red);

    action = Action::IDLE;
    direction  = Direction::RIGHT;
}


void Square::move(float delta)
{
    if (y < 300)
            vely += grav;
    else if (y>300)
        y = 300;

    velx += accx;
    vely += accy;

    x += velx;
    y += vely;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        // TODO: Globe Rotote Clockwise
        direction = Direction::RIGHT;
        action = Action::WALKING;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        // TODO: Globe Rotote Counter-Clockwise
        direction = Direction::LEFT;
        action = Action::WALKING;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        action = Action ::JUMP;
        // TODO: Jump
        vely = -25;
    }

    else {
        action = Action::IDLE;
    }



    // Keep the box within screen borders
    x = std::max(x, 0.f);
    x = std::min(x, (float)(config.screenWidth - size));
    y = std::max(y, 0.f);
    y = std::min(y, (float)(config.screenHeight - size));

    switch(action){
        case Action::WALKING: {
            if(direction== Direction::LEFT){
                rectangle.setFillColor(sf::Color::Blue);
                std::cout << "Walking Left" << std::endl;
            } else {
                rectangle.setFillColor(sf::Color::Red);
                std::cout << "Walking Right" << std::endl;
            }
            break;
        }

        case Action::IDLE:{
            std::cout << "Idle" << std::endl;
            break;
        }

        case Action::JUMP:{

            std::cout << "Jump" << std::endl;
            break;
        }
    }
}

void Square::draw()
{
    // Position the rectangle used to draw the square
    rectangle.setPosition(x, y);

    // Draw the square
    window.draw(rectangle);
}