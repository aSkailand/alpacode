#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

class Alpaca {
public:
    /// Public Functions
    /**
      * CONSTRUCTOR: Creates an alpaca and gives it an initial position.
      * @param stateMachine a reference to the stateMachine, used to access common resources.
      * @param initAngle the angle the alpaca will start at.
      */
    Alpaca(StateMachine &stateMachine, float initAngle);

    /**
     * Randomize the alpaca's action and direction. Afterwards updates the position and rotation
     * according to the alpaca's current state.
     */
    void control();

    /**
     * Updates the alpaca position and then draws it
     */
    void draw();

private:

    /// Enums
    enum class Direction {
        LEFT, RIGHT
    };
    enum class Action {
        IDLE, WALKING
    };
    Direction currentDirection;
    Action currentAction;

    /// Pointers
    sf::RenderWindow *window;
    ConfigGame *configGame;

    /// Alpaca properties
    sf::RectangleShape alpacashape;
    sf::Texture alpacaTexture;
    float x;
    float y;
    float angle;
    int size = 150;
    const float speed = 10;
    const int id;
    static int nextId;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();

    /// Alpaca movement randomizer tools
    sf::Clock clock;
    int tickSecond = 3; // Amount of seconds before a new, random action is given.
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
