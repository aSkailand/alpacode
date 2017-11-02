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
        EMPTY, HOLDING, THROWING
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

    float walkingForce = 5.f;
    float walkingAngle = 45.f;   // Right, Degrees

    float jumpingForce = 10.f;
    float jumpingAngle = 45.f;   // Right, Degrees

    float throwingForce = 10.f;
    float throwingAngle = 10.f;   // Right, Degrees

    // Non-customizable
    sf::Texture texture;
    b2Vec2 rightWalkVec{};
    b2Vec2 leftWalkVec{};

    b2Vec2 rightJumpVec{};
    b2Vec2 leftJumpVec{};

    b2Vec2 rightThrowingVec{};
    b2Vec2 leftThrowingVec{};

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
    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;
};

#endif //ALPACGAME_FARMER_H
