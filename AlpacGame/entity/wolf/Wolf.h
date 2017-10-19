#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <random>
#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

class Wolf {
public:

    int wolfID = 0;

    Wolf(StateMachine &stateMachine, float initAngle);


    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action {
        IDLE, WALKING
    };


    /**
     * Calculate the new position of the alpaca and generates random actions for the wolf
     */
    void control();

    /**
     * Updates the wolf position and then draws it
     */
    void draw();

    /**
     * generates a random number for given numbers
     * @param lower number
     * @param upper number
     * @return returns a number between the lower and upper
     */
    int randomNumberGenerator(int lower, int upper);

    int getTickSecond() const;

    // Wolf properties
    float x;
    float y;
    float angle;
    sf::RectangleShape wolfRectangle;

    // Clock and time needed for timing the random actions of the wolf
    sf::Time elapsedTime;
    sf::Clock clock;

private:

    // Pointers
    ConfigGame *configGame;
    sf::RenderWindow *window;

    const int spriteOffset = 100;
    // The size for the rectangle of the wolf
    const int size = 100;

    // Enum
    Direction currentDirectionState;
    Action currentActionState;

    // How often the wolf will do random actions
    int tickSecond = 3;
    // How fast the wolf runs
    const float speed = 40;

    sf::Texture wolfTexture;
    sf::Sprite wolfSprite;


};

#endif //ALPACGAME_WOLFSTATE_H
