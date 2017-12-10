//
// Created by Trong on 07/11/2017.
//

#ifndef ALPACGAME_SHOTGUN_H
#define ALPACGAME_SHOTGUN_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"
#include "../Usable.h"
#include "../Holdable.h"

class Shotgun : public EntityCold, public Usable, public Holdable {
public:

    void update() override;

    void performHold() override;

    void performThrow() override;

    // todo add Shotgun reload cooldown
    void pause() override;
    void resume() override;

    float length;

    b2World *world;

    Shotgun(ConfigGame *configGame, float length, float height, float x, float y);

    void render(sf::RenderWindow *window) override;

    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void use() override;

    void shootBullets(float bulletForce, float coneAngle, int numBullets);

    bool deadCheck() override;
};


#endif //ALPACGAME_SHOTGUN_H
