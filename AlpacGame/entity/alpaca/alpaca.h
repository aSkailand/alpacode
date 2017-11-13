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
     * @param radius the width of the alpaca in pixels.
     * @param height the height of the alpaca in pixels.
     * @param x the x-coordinate of the origin of the alpaca, in pixels.
     * @param y the y-coordinate of the origin of the alpaca, in pixels.
     */
    Alpaca(b2World *world, ConfigGame *configGame, float radius, float x, float y);

    bool farmerTouch = false;

    //b2FixtureDef getBodySensor() const { return bodySensor; };

private:

    /// AI Status
    enum class Behavior{NORMAL, AFRAID};
    Behavior currentBehavior = Behavior::NORMAL;

    /// Sensors
    b2FixtureDef bodySensor;

    /// Entity properties
    const int id;
    static int nextId;

    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;

    uint16 categoryBits = (uint16) ID::ALPACA;
    uint16 maskBits = (uint16) ID::PLANET | (uint16) ID::WOLF;

    float walkForce = 5.f;
    float walkAngle = 70.f;   // Right, Degrees

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

    /// Pointers
    ConfigGame *configGame;

public:

    /// Body Sensor Contact
    void startContact(CollisionID typeFixture, Entity *contactEntity) override;
    void endContact(Entity *contactEntity) override ;




};

#endif //ALPACGAME_ALPACA_H
