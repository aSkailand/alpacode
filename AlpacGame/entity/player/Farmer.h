#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>

#include <SFML/Graphics.hpp>
#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"
#include "../../state/StateGame.h"

class Farmer {
public:
    /// Public Functions
    /**
      * CONSTRUCTOR: Creates a farmer and gives it an initial position.
      * @param stateMachine a reference to the stateMachine, used to access common resources.
      * @param initAngle the angle the alpaca will start at.
      */
    Farmer(StateMachine &stateMachine, float initAngle);

    /**
     * Randomize the farmer's action and direction. Afterwards updates the position and rotation
     * according to the farmer's current state.
     */
    void control();

    /**
     * Updates the farmer position and then draws it
     */
    void draw();

private:

    /// Enums
    enum class Direction{
        RIGHT, LEFT
    };
    enum class Action{
        IDLE, WALKING, JUMP
    };
    enum class Status{
        GROUNDED, AIRBORNE
    };
    Direction currentDirection;
    Action currentAction;
    Status currentStatus;

    /// Pointers
    sf::RenderWindow *window;
    ConfigGame *configGame;

    /// Alpaca properties
    sf::RectangleShape farmer;
    sf::Texture farmerTexture;
    float x;
    float y;
    float angle;
    const int size = 100;
    float speed = 50;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();
};


#endif //ALPACGAME_FARMER_H
