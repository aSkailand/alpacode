//
// Created by Trong on 09/11/2017.
//

#ifndef ALPACGAME_BULLET_H
#define ALPACGAME_BULLET_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"

class Bullet : public EntityCold{
public:

    Bullet(b2World *world, ConfigGame *configGame, float radius, b2Vec2 position);

    void render(sf::RenderWindow *window) override;

    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;
};


#endif //ALPACGAME_BULLET_H
