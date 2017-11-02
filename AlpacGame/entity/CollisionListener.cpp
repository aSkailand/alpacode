
#include "CollisionListener.h"

void CollisionListener::BeginContact(b2Contact *contact) {

    this->contact = contact;

    //    Check if the body of the fixture has any user data
    Entity *entity_A = getEntity_A();
    Entity *entity_B = getEntity_B();
    Entity::ID ID_A = getID_FixtureA();
    Entity::ID ID_B = getID_FixtureB();

    entity_A->startContact(entity_B);
    entity_B->startContact(entity_A);


}

void CollisionListener::EndContact(b2Contact *contact) {

    this->contact = contact;

    //    Check if the body of the fixture has any user data
    Entity *entity_A = getEntity_A();
    Entity *entity_B = getEntity_B();
    Entity::ID ID_A = getID_FixtureA();
    Entity::ID ID_B = getID_FixtureB();

    entity_A->endContact(entity_B);
    entity_B->endContact(entity_A);
}

Entity::ID CollisionListener::getID_FixtureA() {
    return static_cast<Entity *> (this->contact->GetFixtureA()->GetBody()->GetUserData())->getID();
}

Entity::ID CollisionListener::getID_FixtureB() {
    return static_cast<Entity *> (this->contact->GetFixtureB()->GetBody()->GetUserData())->getID();
}

Entity *CollisionListener::getEntity_A() {
    return static_cast<Entity *> (this->contact->GetFixtureA()->GetBody()->GetUserData());
}

Entity *CollisionListener::getEntity_B() {
    return static_cast<Entity *> (this->contact->GetFixtureB()->GetBody()->GetUserData());
}


