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
public:

    void pause() override;

    void resume() override;

    virtual ~Wolf();

    /**
     * CONSTRUCTOR: Creates a wolf and adds it to the world.
     * @param world the world to add the wolf to.
     * @param configGame the pointer to the configurations.
     * @param radius the radius of the fixture.
     * @param x the x-coordinate of the origin of the wolf, in pixels.
     * @param y the y-coordinate of the origin of the wolf, in pixels.
     */
    Wolf(ConfigGame *configGame, float radius, float width, float height, float x, float y);

    static int nextId;

    bool deadCheck() override;

    void switchCurrentTexture() override;

    /// Stun properties

    bool isStunned = false;
    void performStun();
    void removeStun();

private:
    /// Entity Properties
    const int id;
    float density = 1.0f;
    float friction = 1.0f;
    float restitution = 0.0f;

    float walkForce = 5.f;
    float walkAngle = 30.f;   // Right, Degrees

    float attackForce = 8.f;
    float attackAngle = 25.f;

public:
    void renderDebugMode() override;

private:


    /// Pointers
    ConfigGame *configGame = nullptr;

    /// Behavior
    enum class Behavior{NORMAL, HUNTING, HOMERUN};
    Behavior currentBehavior;

    /// Wolf Base
    b2Vec2 wolfDenVec2;
    sf::CircleShape *wolfDen_Debug = nullptr;

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
