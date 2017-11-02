#ifndef ALPACGAME_FARMER_H
#define ALPACGAME_FARMER_H

#include <iostream>

#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"

class Farmer : public EntityWarm {

public:

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

    /// Farmer properties
    sf::Texture farmerTexture;

    /**
     * The time before next movement is permitted to be performed.
     */
    float moveAvailableTick = 0.4f;

    /// Visuals
    /**
     * Load necessary textures.
     */
    void loadTextures();
};

#endif //ALPACGAME_FARMER_H
