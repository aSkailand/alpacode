
#include "CollisionListener.h"

void CollisionListener::BeginContact(b2Contact *contact) {

    // Get both entities that just start touch
    Entity *entity_A = getEntity_A(contact);
    Entity *entity_B = getEntity_B(contact);

    // Give each entity the info about the other
    entity_A->startContact(entity_B);
    entity_B->startContact(entity_A);

}

void CollisionListener::EndContact(b2Contact *contact) {

    // Get both entities that just lost touch
    Entity *entity_A = getEntity_A(contact);
    Entity *entity_B = getEntity_B(contact);

    // Give each entity the info about the other
    entity_A->endContact(entity_B);
    entity_B->endContact(entity_A);
}

Entity *CollisionListener::getEntity_A(b2Contact *contact) {
    return static_cast<Entity *> (contact->GetFixtureA()->GetBody()->GetUserData());
}

Entity *CollisionListener::getEntity_B(b2Contact *contact) {
    return static_cast<Entity *> (contact->GetFixtureB()->GetBody()->GetUserData());
}


