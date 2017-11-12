//
// Created by Trong on 09/11/2017.
//

#ifndef ALPACGAME_BULLET_H
#define ALPACGAME_BULLET_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"

class Bullet : public EntityCold{
public:

    ConfigGame *configGame;

    ~Bullet();

    b2Filter filter;

    bool hit = false;

    Bullet(b2World *world, ConfigGame *configGame, float radius, b2Vec2 position);

    void render(sf::RenderWindow *window) override;

    void startContact(Entity *contactEntity) override;

    void endContact(Entity *contactEntity) override;

    bool deadCheck() override;

private:
    sf::Clock decayClock;
    float decayTick = 1.0f;
};


#endif //ALPACGAME_BULLET_H
