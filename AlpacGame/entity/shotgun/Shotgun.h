//
// Created by Trong on 07/11/2017.
//

#ifndef ALPACGAME_SHOTGUN_H
#define ALPACGAME_SHOTGUN_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"
#include "../../Resources/ConfigSound.h"
#include "../Usable.h"
#include "../Holdable.h"

class Shotgun : public EntityCold, public Usable, public Holdable {
public:

//    bool farmerTouch = false;

//    bool isHeld = false;

    float length;

    ConfigGame *configGame;

    ConfigSound *configSound;

    b2World *world;

    Shotgun(ConfigGame *configGame, ConfigSound *configSound, float length, float height, float x, float y);

    void render(sf::RenderWindow *window) override;

    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void use() override;

    void shootBullets(float bulletForce, float coneAngle, int numBullets);

    bool deadCheck() override;
};


#endif //ALPACGAME_SHOTGUN_H
