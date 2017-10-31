#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"
#include "../Entity.h"

class Alpaca : public Entity{
public:
    /// Public Functions
    Alpaca(b2World *world, b2Body *planetBody, float width, float height, float x, float y);

    void adjust() override;

    /// Enums
    enum class Direction {
        LEFT, RIGHT
    };

private:

    b2Body *planetBody;

    enum class Action {
        IDLE, WALKING
    };
    Direction currentDirection;
    Action currentAction;

    /// Pointers
    sf::RenderWindow *window;
    ConfigGame *configGame;

    /// Alpaca properties
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
    float tickSecond = 3.f; // Amount of seconds before a new, random action is given.
    std::default_random_engine generator;

    /**
    * Generates a random number from the generator in the range of given lower and upper.
    * @param lower the left number in the range.
    * @param upper the right number in the range.
    * @return returns a number between the lower and upper.
    */
    int randomNumberGenerator(int lower, int upper);

    void switchAction() override;
};

#endif //ALPACGAME_ALPACA_H
