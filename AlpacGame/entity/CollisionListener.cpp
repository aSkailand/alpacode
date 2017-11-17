
#include "CollisionListener.h"

void CollisionListener::BeginContact(b2Contact *contact) {


    // Get both entities that just start touch
    Entity *entity_A = getEntity_A(contact);
    Entity *entity_B = getEntity_B(contact);

    // Get the Sensor Types [HIT, DETECT]
    Entity::CollisionID typeCollision_A = getCollisionType_A(contact);
    Entity::CollisionID typeCollision_B = getCollisionType_B(contact);

    // Give each entity the info about the other
    entity_A->startContact(typeCollision_A, typeCollision_B, entity_B);
    entity_B->startContact(typeCollision_B, typeCollision_A, entity_A);


}

void CollisionListener::EndContact(b2Contact *contact) {

    // Get both entities that just lost touch
    Entity *entity_A = getEntity_A(contact);
    Entity *entity_B = getEntity_B(contact);

    // Get the Sensor Types [HIT, DETECT]
    Entity::CollisionID typeCollision_A = getCollisionType_A(contact);
    Entity::CollisionID typeCollision_B = getCollisionType_B(contact);

    // Give each entity the info about the other
    entity_A->endContact(typeCollision_A, typeCollision_B, entity_B);
    entity_B->endContact(typeCollision_B, typeCollision_A, entity_A);
}

Entity *CollisionListener::getEntity_A(b2Contact *contact) {
    return static_cast<Entity *> (contact->GetFixtureA()->GetBody()->GetUserData());
}

Entity *CollisionListener::getEntity_B(b2Contact *contact) {
    return static_cast<Entity *> (contact->GetFixtureB()->GetBody()->GetUserData());
}

Entity::CollisionID CollisionListener::getCollisionType_A(b2Contact *contact) {
    return static_cast<Entity::CollisionID> (Entity::convertToCollisionID(contact->GetFixtureA()->GetUserData()));
}

Entity::CollisionID CollisionListener::getCollisionType_B(b2Contact *contact) {
    return static_cast<Entity::CollisionID> (Entity::convertToCollisionID(contact->GetFixtureB()->GetUserData()));
}


