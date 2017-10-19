#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <random>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>

class Wolf {
public:

    Wolf(sf::RenderWindow &renderWindow);

    //constructor dependancy inject render window
    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action{
        IDLE, WALKING
    };

    Action getCurrentActionState();
    void control(float rotationDelta);
    void draw();
    int randomNumberGenerator(int lower, int upper);
    int getTickSecond() const;

    float x;
    float y;

    sf::Clock clock;
    sf::Time elapsedTime;
    sf::RectangleShape wolfRectangle;


private:
    const int size = 100;
    Direction currentDirectionState;
    Action currentActionState;
    int tickSecond = 3;

    sf::RenderWindow *window;
    sf::VideoMode windowSize;
    sf::Texture wolfTexture;


};

#endif //ALPACGAME_WOLFSTATE_H
