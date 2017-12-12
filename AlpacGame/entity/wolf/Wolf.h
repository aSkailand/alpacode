#ifndef ALPACGAME_WOLFSTATE_H
#define ALPACGAME_WOLFSTATE_H

#include <iostream>
#include <random>
#include <chrono>
#include <list>

#include "../../Resources/ConfigGame.h"

#include "../EntityWarm.h"
#include "../Mob.h"

class Trap;

class Wolf : public Mob{

private:
    /// Entity Properties
    const int id;
    float radius = 40.f;
    float width = 150.f;
    float height = 100.f;

    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;

    float walkForce = 5.f;
    float walkAngle = 30.f;   // Right, Degrees

    float attackForce = 8.f;
    float attackAngle = 25.f;

    float enteringDenTick = 2.f;

    float detectionRadius = 200.f;

public:

    void pause() override;

    void resume() override;

    ~Wolf() override;

    /**
     * CONSTRUCTOR: Creates a wolf and adds it to the world.
     * @param world the world to add the wolf to.
     * @param configGame the pointer to the configurations.
     * @param radius the radius of the fixture.
     * @param x the x-coordinate of the origin of the wolf, in pixels.
     * @param y the y-coordinate of the origin of the wolf, in pixels.
     */
    Wolf(ConfigGame *configGame, float x, float y);

    static int nextId;

    bool deadCheck() override;

    void switchCurrentTexture() override;

    /// Stun properties

    bool isStunned = false;
    void performStun();
    void removeStun();

    /// Render Debug
    void renderDebugMode() override;

private:

    /// Behavior
    enum class Behavior{HUNTING, CHASING, RETREATING};
    Behavior currentBehavior = Behavior::HUNTING;
    sf::RectangleShape alertSteakIndicator;
    sf::CircleShape *wolfDen_Debug = nullptr;

    /** HUNTING
    *  1. Moves to one direction (Random) until an entity is detected
    *  2. Adds the entity in a list<Entity *>
    *  3. Iterates list, finds the lowest Length() value and follows it
    */
    /// Wolf AttackVector
    std::list<Entity *> currentlyDetectedEntities;

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

    /// Movement tools
    /**
     * The time before current action is switched (in seconds).
     */
    float randomActionTick = 3.0f;

    /**
     * The time before next movement is permitted to be performed (in seconds).
     */
    float moveAvailableTick = 0.4f;

    /// Animation tools
    bool spriteSwitch = false;
    std::map<EntityWarm::Action , SpriteInfo> wolfTextureMap;

    /// Chrono Clock
    sftools::Chronometer homeTimer;


public:

    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;
    void startContact_body(CollisionID otherCollision, Entity *contactEntity);
    void startContact_hit(CollisionID otherCollision, Entity *contactEntity);
    void startContact_detection(CollisionID otherCollision, Entity *contactEntity);

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;
    void endContact_body(CollisionID otherCollision, Entity *contactEntity);
    void endContact_hit(CollisionID otherCollision, Entity *contactEntity);
    void endContact_detection(CollisionID otherCollision, Entity *contactEntity);
};

#endif //ALPACGAME_WOLFSTATE_H
