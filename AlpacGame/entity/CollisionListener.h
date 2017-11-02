
#ifndef ALPACGAME_COLLISIONLISTENER_H
#define ALPACGAME_COLLISIONLISTENER_H

#include <iostream>

#include "Entity.h"

class CollisionListener : public b2ContactListener {

public:
    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;

    b2Contact *contact;

    Entity::ID getID_FixtureA();
    Entity::ID getID_FixtureB();

    Entity* getEntity_A();
    Entity* getEntity_B();

};


#endif //ALPACGAME_COLLISIONLISTENER_H
