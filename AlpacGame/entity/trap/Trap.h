
#ifndef ALPACGAME_TRAP_H
#define ALPACGAME_TRAP_H

#include <list>

#include "../../Resources/ConfigGame.h"

#include "../EntityCold.h"

#include "../Usable.h"
#include "../Holdable.h"

#include "../wolf/Wolf.h"

#include "../cooldown/CooldownBarometer.h"

/**
 * Entity Trap, handling the creation and logic of the trap used in-game.
 */
class Trap : public EntityCold, public Holdable {

public:

    Trap(ConfigGame *configGame, float length, float height, float x, float y);

    /// Trap time control
    CooldownBarometer *trapCooldownBarometer = nullptr;
    sftools::Chronometer trapClock;
    float stunTick = 10.0f;
    float openTick = 3.0f;

    /// EntityCold Functions
    void render(sf::RenderWindow *window) override;
    void update() override;

    /// Death handling
    bool deadCheck() override;

    /// Holdable functions
    void performHold() override;
    void performThrow() override;

private:

    /// Enums
    enum class Mode {
        CLOSED = 0,
        OPEN = 1,
        READY = 2,
        LATCHED = 3
    };
    Mode currentMode = Mode::CLOSED;

    /// Targeting functions
    std::list<Wolf *> currentlyTouchingWolves;
    Wolf *stunnedTarget = nullptr;
    bool checkIfTouching(Entity *entity);
    bool checkIfTargetIsDead();

    /// Collision functions
    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;
    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    /// Pointers
    ConfigGame *configGame = nullptr;

public:
    void pause() override;

    void resume() override;
};


#endif //ALPACGAME_TRAP_H
