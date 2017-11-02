#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>
#include <chrono>

#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"
#include "../Mob.h"

class Alpaca : public Mob {
public:

    /**
     * CONSTRUCTOR: Creates an alpaca and adds it to the world.
     * @param world the world to add the alpaca to.
     * @param width the width of the alpaca in pixels.
     * @param height the height of the alpaca in pixels.
     * @param x the x-coordinate of the origin of the alpaca, in pixels.
     * @param y the y-coordinate of the origin of the alpaca, in pixels.
     */
    Alpaca(b2World *world, float width, float height, float x, float y);

private:

    /// Entity properties
    sf::Texture texture;
    const int id;
    static int nextId;
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;
    uint16 categoryBits = (uint16) ID::ALPACA;
    uint16 maskBits = (uint16) ID::PLANET;

    /// Movement tools
    /**
     * The time before current action is switched (in seconds).
     */
    float randomActionTick = 3.0f;

    /**
     * The time before next movement is permitted to be performed (in seconds).
     */
    float moveAvailableTick = 0.5f;

    /// Functions
    /**
     * Adjust SFML shape accordingly to the Box2D body, then draw it.
     * @param window the window to draw the SFML shape on.
     */
    void render(sf::RenderWindow *window) override;

    /**
     * Perform the current action if permitted by move clock.
     */
    void performAction() override;

    /**
     * Randomize the entity's current action and direction.
     */
    void switchAction() override;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();


};

#endif //ALPACGAME_ALPACA_H
