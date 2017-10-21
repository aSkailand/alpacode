#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

// todo: Add a way to distinguish alpacas. Perhaps a static int id?

class Alpaca {
public:
    // Enums
    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action {
        IDLE, WALKING
    };

    // Alpaca properties
    sf::RectangleShape alpaca;
    sf::Texture alpacaTexture;
    float x, y;
    float angle;
    const int size = 150;
    const float speed = 10;

    /**
     * CONSTRUCTOR: Creates an alpaca and place it on the planet in the given position.
     * @param stateMachine a reference to the stateMachine, used to access common resources.
     * @param initAngle the angle the alpaca will start at.
     */
    Alpaca(StateMachine &stateMachine, float initAngle);

    /**
     * Calculate the new position of the alpaca.
     */
    void control();

    /**
     * Update position and rotation of the alpaca, then draw it.
     */
    void draw();

private:

    // Pointers
    sf::RenderWindow *window;
    ConfigGame *configGame;

    // Incrementing id variable
    static int nextId;

    // Current enum
    Direction currentDirection;
    Action currentAction;

    // Clock
    sf::Clock clock;

    /**
     * Amount of seconds the alpaca waits before performing the next change of action.
     */
    float actionTick = 3;

    // Helper Functions
    /**
     * Randomize the current action, and perform it.
     */
    void randomAction();

    /**
     * Randomize random direction, and perform it.
     */
    void randomDirection();

    /**
     * Load all the necessary textures.
     */
    void loadTextures();

    // Random Number Generator
    std::default_random_engine generator;

    /**
    * Generates a random number from the generator in the range of given lower and upper.
    * @param lower the left number in the range.
    * @param upper the right number in the range.
    * @return returns a number between the lower and upper.
    */
    int randomNumberGenerator(int lower, int upper);
};

#endif //ALPACGAME_ALPACA_H
