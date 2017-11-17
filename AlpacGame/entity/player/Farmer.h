#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>
#include <list>

#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"

class Farmer : public EntityWarm {

public:

    /**
     * CONSTRUCTOR: Creates a farmer and places in to the world.
     * @param world the world to add the farmer to.
     * @param configGame the configurations of the game.
     * @param radius the radius of the fixture shape of entity.
     * @param x the x-position of the farmer's origin, in pixels.
     * @param y the y-position of the farmer's origin, in pixels.
     */
    Farmer(b2World *world, ConfigGame *configGame, float radius, float x, float y);

private:

    /// Entity properties
    // Customizable
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;

    uint16 categoryBits = (uint16) ID::FARMER;
    uint16 maskBits = (uint16) ID::PLANET | (uint16) ID::WOLF;

    float walkForce = 5.f;
    float walkAngle = 45.f;   // Right, Degrees

    float jumpForce = 12.f;
    float jumpAngle = 60.f;   // Right, Degrees

    float throwForce = 10.f;
    float throwAngle = 10.f;   // Right, Degrees

    /**
     * The time before next movement is permitted to be performed.
     * Slight delay due to calculations are too fast.
     */
    float moveAvailableTick = 0.1f;

    /// Grasp Properties
    enum class Grasp {
        EMPTY, HOLDING, THROWING = 3
    };

    Entity *holdingEntity = nullptr;
    sf::Clock graspClock;
    float graspCooldown = 0.1f;
    Grasp currentGrasp = Grasp::EMPTY;
    std::list<Entity *> currentlyTouchingEntities;

    /// Functions
    /**
     * Adjust SFML shape accordingly to the Box2D body, then draw it.
     * @param window the window to draw the SFML shape on.
     */

    void render(sf::RenderWindow *window) override;

    /**
     * Perform the current action.
     */
    void performAction() override;

    /**
     * Switch current action and direction accordingly to the current inputs.
     */
    void switchAction() override;

    /// Pointers
    ConfigGame *configGame;



public:

    void startContact( CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

};

#endif //ALPACGAME_FARMER_H
