
#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Farmer {
public:

    enum class Action{IDLE, WALKING, JUMP};
    enum class Status{GROUNDED, AIRBORNE};
    enum class Direction{RIGHT, LEFT};

    Farmer(sf::RenderWindow &renderWindow);

    sf::RectangleShape rectangle;

    float velocity_y;
    float y;
    float gravity = 3;

    void control(float rotationDelta);

    void draw();

    Action action;
    Status status;
    Direction direction;


private:
    const int size = 100;

    sf::RenderWindow *window;
    sf::VideoMode windowSize;
    sf::Texture farmerTexture;
};


#endif //ALPACGAME_FARMER_H
