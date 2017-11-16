//
// Created by Trong on 07/11/2017.
//

#ifndef ALPACGAME_SHOTGUN_H
#define ALPACGAME_SHOTGUN_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"
#include "../Usable.h"

class Shotgun : public EntityCold, public Usable {
public:

    bool farmerTouch = false;

    bool held = false;

    float length;

    ConfigGame *configGame;

    b2World *world;

    Shotgun(ConfigGame *configGame, float length, float height, float x, float y);

    void render(sf::RenderWindow *window) override;

    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;

    void use() override;

    void shootBullets(float bulletForce, float coneAngle, int numBullets);

    bool deadCheck() override;
};


#endif //ALPACGAME_SHOTGUN_H
