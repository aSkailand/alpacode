//
// Created by Trong on 07/11/2017.
//

#ifndef ALPACGAME_SHOTGUN_H
#define ALPACGAME_SHOTGUN_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"

class Shotgun : public EntityCold{
public:
    Shotgun(b2World *world, ConfigGame *configGame, float width, float height, float x, float y);

    void render(sf::RenderWindow *window) override;

    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;
};




#endif //ALPACGAME_SHOTGUN_H
