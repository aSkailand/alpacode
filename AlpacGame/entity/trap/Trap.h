
#ifndef ALPACGAME_TRAP_H
#define ALPACGAME_TRAP_H


#include "../EntityCold.h"
#include "../Usable.h"
#include "../Holdable.h"
#include "../wolf/Wolf.h"
#include "../../Resources/ConfigGame.h"

class Trap : public EntityCold, public Holdable {

public:

    enum class Mode {
        CLOSED = 0,
        OPEN = 1,
        READY = 2,
        LATCHED = 3
    };

    Mode currentMode = Mode::CLOSED;

    Status currentStatus = Status::AIRBORNE;

    void update() override;

public:

    sftools::Chronometer trapClock;
    float stunTick = 10.0f;
    float readyTick = 0.0f;
    float openTick = 5.0f;

    void performStun();

    bool activated = false;
    Wolf *stunnedTarget = nullptr;

    void deleteStunnedTarget(){
        delete stunnedTarget;
        stunnedTarget = nullptr;
    }

    Trap(ConfigGame *configGame, float length, float height, float x, float y);

    void render(sf::RenderWindow *window) override;

    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    bool deadCheck() override;

    void performHold() override;

    void performThrow() override;

private:

    std::list<Wolf *> currentlyTouchingEntities;

    bool checkIfTouching(Entity *entity);

    ConfigGame *configGame = nullptr;

};


#endif //ALPACGAME_TRAP_H
