#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>
#include <list>

#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"

class Farmer : public EntityWarm {

public:

    sf::Clock graspClock;
    float graspCooldown = 0.2f;

    enum class Grasp {
        EMPTY, HOLDING, THROWING = 3
    };

    Grasp currentGrasp = Grasp::EMPTY;
    Entity *holdingEntity = nullptr;
    std::list<Entity *> currentlyTouchingEntities;

    /**
     * CONSTRUCTOR: Creates a farmer and places in to the world.
     * @param world the world to add the farmer to.
     * @param configGame the configurations of the game.
     * @param width the width of the farmer in pixels.
     * @param height the height of the farmer in pixels.
     * @param x the x-position of the farmer's origin, in pixels.
     * @param y the y-position of the farmer's origin, in pixels.
     */
    Farmer(b2World *world, ConfigGame *configGame, float width, float height, float x, float y);

private:

    /// Entity properties
    // Customizable
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;

    uint16 categoryBits = (uint16) ID::FARMER;
    uint16 maskBits = (uint16) ID::PLANET;

    float walkForce = 5.f;
    float walkAngle = 45.f;   // Right, Degrees

    float jumpForce = 10.f;
    float jumpAngle = 45.f;   // Right, Degrees

    float throwForce = 10.f;
    float throwAngle = 10.f;   // Right, Degrees

    // Non-customizable
    sf::Texture texture;

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

    /**
     * The time before next movement is permitted to be performed.
     */
    float moveAvailableTick = 0.4f;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();

public:


    b2Vec2 walkVec[2]{};
    b2Vec2 jumpVec[2]{};
    b2Vec2 throwVec[2]{};

    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;
};

#endif //ALPACGAME_FARMER_H
