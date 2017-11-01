#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>

#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"
#include "../../state/StateGame.h"

#include "../EntityWarm.h"

class Farmer : public EntityWarm {

public:

    /// Public Functions
//    /**
//      * CONSTRUCTOR: Creates a farmer and gives it an initial position.
//      * @param stateMachine a reference to the stateMachine, used to access common resources.
//      * @param initAngle the angle the alpaca will start at.
//      */
    Farmer(b2World *world, ConfigGame *configGame, float width, float height, float x, float y);
    /// Farmer properties
    sf::Texture farmerTexture;


//    /**
//     * Randomize the farmer's action and direction. Afterwards updates the position and rotation
//     * according to the farmer's current state.
//     */
//    void switchAction();

    void adjust() override;

    void switchAction() override;

//    /**
//     * Updates the farmer position and then draws it
//     */
//    void draw();

    sf::Clock moveTimer;
    float moveCoolDown = 0.4f;

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




    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();
};


#endif //ALPACGAME_FARMER_H
