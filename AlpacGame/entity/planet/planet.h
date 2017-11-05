
#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>

#include "../../state/StateGame.h"

#include "../EntityCold.h"

class Planet : public EntityCold{
public:

    /**
     * CONSTRUCTOR: Create a planet and adds it to the window.
     * @param world the world to add the world to.
     * @param radius the radius of the planet in pixels.
     * @param x the x-position of the origin of the planet, in pixels.
     * @param y the y-position of the origin of the planet, in pixels.
     */
    Planet(b2World *world, ConfigGame *configGame, float radius, float x, float y);

private:

    /**
     * Adjust SFML shape accordingly to the Box2D body, then draw it.
     * @param window the window to draw the SFML shape on.
     */
    void render(sf::RenderWindow *window) override;

    /// Entity properties
    float density = 1.0f;
    float friction = 5.0f;
    float restitution = 0.0f;

    uint16 categoryBits = (uint16) ID::PLANET;
    uint16 maskBits = (uint16) ID::FARMER | (uint16) ID::ALPACA | (uint16) ID::WOLF;


public:

    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;

};

#endif