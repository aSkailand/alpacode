#ifndef ALPACGAME_ALPACA_H
#define ALPACGAME_ALPACA_H

#include <iostream>
#include <random>
#include <chrono>
#include <list>

#include "../../state/StateMachine.h"
#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"
#include "../Mob.h"
#include "../Holdable.h"

class Alpaca : public Mob, public Holdable {
public:

    /// Breeding Logic
    bool isAdult = false;
    bool isFertile = false;
    float radius = 40.f;
    sf::Vector2f sizeAdult = sf::Vector2f(100.f, 100.f);
    sf::Vector2f sizeKid   = sf::Vector2f(70.f, 70.f);

    int max_HP = 3;

    void adultify();

    void pause() override;

    void resume() override;

    ~Alpaca() override;

    /**
     * CONSTRUCTOR: Creates an alpaca and adds it to the world.
     * @param world the world to add the alpaca to.
     * @param radius the width of the alpaca in pixels.
     * @param height the height of the alpaca in pixels.
     * @param x the x-coordinate of the origin of the alpaca, in pixels.
     * @param y the y-coordinate of the origin of the alpaca, in pixels.
     */
    Alpaca(ConfigGame *configGame, bool isAdult, float x, float y);

    static int nextId;

    void switchCurrentTexture() override;

    void renderDebugMode() override;

    void performHold() override;

    void performThrow() override;

private:

    sf::RectangleShape sf_fertileHeart;

    /// AI Behavior
    enum class Behavior {
        NORMAL, AWARE, AFRAID
    };
    Behavior currentBehavior;

private:

    /// Entity properties
    const int id;

    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;

    float walkForce = 5.f;
    float walkAngle = 70.f;   // Right, Degrees

    float runForce = 7.f;

    float detectionRadius = 300.f;

    /// Movement tools
    /**
     * The time before current action is switched (in seconds).
     */
    float randomActionTick = 3.0f;

    /**
     * The time for Alpaca to change behaviors
     */
    float awareActionTick = 0.5f;
    float afraidActionTick = 3.0f;

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

public:
    bool deadCheck() override;

private:

    std::list<Entity * > currentDetectedEntity;

    uint16 categoryBits = (uint16) ID::ALPACA;

    uint16 maskBits = (uint16) ID::PLANET
                      | (uint16) ID::WOLF
                      | (uint16) ID::BULLET;

    std::map<EntityWarm::Action, SpriteInfo> alpacaMapPtr;

    sftools::Chronometer behaviorClock;

public:

    /// Body Sensor Contact


    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void startContact_hit(CollisionID otherCollision, Entity *contactEntity);

    void startContact_body(CollisionID otherCollision, Entity *contactEntity);

    void startContact_detection(CollisionID otherCollision, Entity *contactEntity);

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact_hit(CollisionID otherCollision, Entity *contactEntity);

    void endContact_body(CollisionID otherCollision, Entity *contactEntity);

    void endContact_detection(CollisionID otherCollision, Entity *contactEntity);

};

#endif //ALPACGAME_ALPACA_H
