
#ifndef ALPACGAME_ENTITY_H
#define ALPACGAME_ENTITY_H

#define SCALE 30.f
#define DEGtoRAD 57.2958f

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Chronometer.hpp>

/**
 * The abstract, base class for all entities.
 * Definition of an entity is:
 *  1. An object affected by the physics provided by Box2D.
 *  2. An object to be rendered to the game window with SFML.
 */
class Entity {

public:

    /// Enums
    enum class ID {
        VOID    = 0x0000,
        PLANET  = 0x0001,
        FARMER  = 0x0002,
        ALPACA  = 0x0004,
        WOLF    = 0x0008,
        SHOTGUN = 0x0010,
        BULLET  = 0x0020,
        TRAP    = 0x0040
    };

    enum class Status {
        GROUNDED, AIRBORNE
    };

    enum class Direction{
        RIGHT = 0,
        LEFT = 1
    };

    enum class CollisionID{
        BODY = 0,
        HIT = 1,
        DETECTION = 2
    };



    /*
     * BOX2D FIXTURE EXPLANATION:
     * Each entity is made of multiple fixtures, which are basically Box2D shapes.
     * Collisions between two entities are actually collisions between fixtures.
     * A fixture can either be a sensor or a non-sensor. A fixture that is a sensor
     * is a fixture without physical properties like density and mass.
     * Depending on the entity type, the fixtures are defined as this:
     *
     * 1. Body Fixture (non-sensor):
     *      Warm Entities: Used to emulate physical collisions logic.
     *      Cold Entities: Used to emulate physical collisions logic.
     *
     * 2. Hit Fixture (sensor):
     *      Warm Entities: Used to determine touching logic when the entity hit box collides.
     *      Cold Entities: Used to determine touching logic when the entity hit box collides.
     *
     * 3. Detection Fixture (sensor):
     *      Warm Entities: Used by mobs to determine detection logic.
     *      Cold Entities: Not used by cold entities.
     *
     * WHY HAVE A FIXTURE FOR BOTH BODY AND HIT:
     * Without the sensor fixture hit, it would be difficult to check if two entities
     * were touching without having them physical collide. E.g. a farmer wants to be
     * able to walk through alpacas, but in the same time be able to tell if he is
     * in vicinity of the alpacas.
     *
     */

    /// Fixture pointers
    b2Fixture *fixture_body = nullptr;
    b2Fixture *fixture_hit = nullptr;
    b2Fixture *fixture_detection = nullptr;

    sf::CircleShape *sf_DebugBody = nullptr;
    sf::CircleShape *sf_DebugHit = nullptr;
    sf::CircleShape *sf_DebugDetection = nullptr;

    /**
     * Adjust the position and rotation of the shape (SFML) to fit the body (Box2D).
     */
    virtual void render(sf::RenderWindow *window) = 0;

    /**
     * Setter for entity type of the child inheriting this class.
     * @param id entity type to set the child to.
     */
    void setEntity_ID(ID id) {
        Entity::entity_ID = id;
    };

    /**
     * Getter for entity type of the child inheriting this class.
     * @return the entity type of the child.
     */
    ID getEntity_ID() const {
        return entity_ID;
    };

    /**
     * Getter for body pointer.
     * @return the entity's body pointer.
     */
    b2Body *getBody() const {
        return body;
    };

    /**
     * Setter for body pointer.
     */
    void setBody(b2Body *body) {
        Entity::body = body;
    }

    /// For Detection
    void* convertToVoidPtr(int enumValue){
        return reinterpret_cast<void *>(enumValue);
    };

    static CollisionID convertToCollisionID(void* voidPtr){
        return (CollisionID&) voidPtr;
    };

    /// Contact Functions
    virtual void startContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) = 0;
    virtual void endContact(CollisionID selfCollision, CollisionID otherCollision, Entity *contactEntity) = 0;

    //virtual void startDetect(Entity *contactEntity) = 0;
    //virtual void endDetect(Entity *contactEntity) = 0;



    /// Death
    virtual bool deadCheck() = 0;

    virtual ~Entity() = default;

protected:

    /// Important Entity Properties

    ID entity_ID = ID::VOID;

    b2Body *body = nullptr;

    sf::Shape *sf_ShapeEntity = nullptr;

    /// Unit Vector + Force functions

    /**
    * Push the given body with the given force in a given angle, in the given direction.
    * @param key the key that is mapped with the correct unit vector.
    * @param targetBody the body to be pushed.
    * @param force the amplitude of the force to push the body with.
    * @param pushDirection the direction to push, either left or right.
    */
    void forcePushBody(int key, b2Body *targetBody, float force, Direction pushDirection) {
        float mass = targetBody->GetMass();
        b2Vec2 angle = targetBody->GetWorldVector(angleVectors.at(key)[(int) pushDirection]);
        targetBody->ApplyLinearImpulseToCenter(force * mass * angle, true);
    }

    /**
     * Convert the given angle (in degrees) to two different unit vectors (b2Vec2):
     *  1. (RIGHT, b2Vec2)  The unit vector spanned by the angle.
     *  2. (LEFT, b2Vec2)   The horizontal flipped version of the first unit vector.
     * Both of these vectors (b2Vec2) are stored inside another vector (std::vector).
     * This vector (std::vector) is stored in the entity's map of unit vectors, with
     * the given key.
     * @param key the key to map the pair of unit vectors with.
     * @param angle the angle to convert over to unit vectors.
     */
    void convertAngleToVectors(int key, float angle) {
        angle /= DEGtoRAD;
        std::vector<b2Vec2> tempVec;
        tempVec.emplace_back(cos(angle), -sin(angle));     // Right Angle
        tempVec.emplace_back(-cos(angle), -sin(angle));    // Left Angle
        angleVectors[key] = tempVec;
    }

private:

    /// Unit Vector Storage

    /**
     * Map storing unit vectors, used to define angles for various forces.
     * All entities have their own version of the map.
     */
    std::map<int, std::vector<b2Vec2>> angleVectors;




};



#endif //ALPACGAME_ENTITY_H
