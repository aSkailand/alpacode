//
// Created by Trong on 09/11/2017.
//

#ifndef ALPACGAME_BULLET_H
#define ALPACGAME_BULLET_H


#include "../EntityCold.h"
#include "../../Resources/ConfigGame.h"

class Bullet : public EntityCold{
public:

    void pause() override;

    void resume() override;

    ~Bullet() override;

    ConfigGame *configGame;

    bool hit = false;

    Bullet(b2World *world, ConfigGame *configGame, float radius, b2Vec2 position);

    void render(sf::RenderWindow *window) override;

    void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) override;

    bool deadCheck() override;

private:
    sftools::Chronometer decayClock;
    float decayTick = 1.0f;
};


#endif //ALPACGAME_BULLET_H
