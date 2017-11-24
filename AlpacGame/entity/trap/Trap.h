
#ifndef ALPACGAME_TRAP_H
#define ALPACGAME_TRAP_H


#include "../EntityCold.h"
#include "../Usable.h"
#include "../Holdable.h"
#include "../../Resources/ConfigGame.h"

class Trap : public EntityCold, public Usable, public Holdable {
public:

    bool activated = false;

    Trap(ConfigGame *configGame, float length, float height, float x, float y);

    void render(sf::RenderWindow *window) override;

    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    bool deadCheck() override;

    void use() override;

private:

    ConfigGame *configGame = nullptr;

};


#endif //ALPACGAME_TRAP_H
