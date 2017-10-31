#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

class Wolf : public Entity{
public:
    /**
      * CONSTRUCTOR: Creates a wolf and gives it an initial position.
      * @param stateMachine a reference to the stateMachine, used to access common resources.
      * @param initAngle the angle the wolf will start at.
      */
    Wolf(b2World *world, b2Body *planetBody, float width, float height, float x, float y);
    /**
     * Randomize the wolf's action and direction. Afterwards updates the position and rotation
     * according to the wolf's current state.
     */
    void switchAction() override;

    /// Public Functions
    void adjust() override;

private:

    b2Body *planetBody;

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

    /// Wolf properties
    sf::Texture wolfTexture;
    float x;
    float y;
    float angle;
    int size = 100;
    const float speed = 40;
    const int id;
    static int nextId;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();

    /// Wolf movement randomizer tools
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

#endif //ALPACGAME_WOLFSTATE_H
