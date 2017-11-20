
#ifndef ALPACGAME_COLLISIONLISTENER_H
#define ALPACGAME_COLLISIONLISTENER_H

#include <iostream>
#include "Entity.h"

/**
 * Collision handler for Box2D collisions.
 * The functions here get called ONLY when a contact between fixtures occur.
 */
class CollisionListener : public b2ContactListener {

private:

    /**
     * This function gets called when two fixtures touch.
     * @param contact the Box2D object handling touch.
     */
    void BeginContact(b2Contact *contact) override;

    /**
     * This function gets called when two fixtures stop touching each other.
     * @param contact the Box2D object handling touch.
     */
    void EndContact(b2Contact *contact) override;

    /**
     * Get entity A of the two contacting entities.
     * @param contact the Box2D object handling touch.
     * @return entity A
     */
    Entity *getEntity_A(b2Contact *contact);

    /**
     * Get entity B of the two contacting entities.
     * @param contact the Box2D object handling touch.
     * @return entity B
     */
    Entity *getEntity_B(b2Contact *contact);

    /**
     * Get CollisionID of the entity A.
     * @param contact the Box2D object handling touch.
     * @return CollisionID of the entity A
     */
    Entity::CollisionID getCollisionType_A(b2Contact *contact);

    /**
     * Get CollisionID of the entity B.
     * @param contact the Box2D object handling touch.
     * @return CollisionID of the entity B
     */
    Entity::CollisionID getCollisionType_B(b2Contact *contact);

};


#endif // ALPACGAME_COLLISIONLISTENER_H
