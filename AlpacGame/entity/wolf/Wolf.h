#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <iostream>
#include <random>
#include <chrono>

#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"
#include "../Mob.h"

class Wolf : public Mob{
public:

    /**
      * CONSTRUCTOR: Creates a wolf and adds it to the world.
      * @param world the world to add the wolf to.
      * @param width the width of the wolf in pixels.
      * @param height the height of the wolf in pixels.
      * @param x the x-coordinate of the origin of the wolf, in pixels.
      * @param y the y-coordinate of the origin of the wolf, in pixels.
      */
    Wolf(b2World *world, float width, float height, float x, float y);

private:

    /// Functions
    /**
     * Randomize the entity's current action and direction.
     */
    void switchAction() override;

    /**
     * Perform the current action if permitted by move clock.
     */
    void performAction() override;

    /**
     * Adjust SFML shape accordingly to the Box2D body, then draw it.
     * @param window the window to draw the SFML shape on.
     */
    void render(sf::RenderWindow *window) override;

    /// Entity Properties
    sf::Texture wolfTexture;
    const int id;
    static int nextId;
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;
    uint16 categoryBits = (uint16) ID::WOLF;
    uint16 maskBits = (uint16) ID::PLANET;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();

    /// Movement tools
    /**
     * The time before current action is switched (in seconds).
     */
    float randomActionTick = 3.0f;

    /**
     * The time before next movement is permitted to be performed (in seconds).
     */
    float moveAvailableTick = 0.4f;

};

#endif //ALPACGAME_WOLFSTATE_H
