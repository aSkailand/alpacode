
#ifndef ALPACGAME_ENTITY_H
#define ALPACGAME_ENTITY_H

#define SCALE 30.f
#define DEGtoRAD 57.2958f

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

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
        PLANET  = 0x0001,
        FARMER  = 0x0002,
        ALPACA  = 0x0004,
        WOLF    = 0x0008,
        SHOTGUN = 0x0010,
        BULLET  = 0x0100
    };

    enum class Direction{
        RIGHT = 0,
        LEFT = 1
    };

    /// Fixture pointers
    b2Fixture *bodyFixture;
    b2Fixture *sensorFixture;

    bool physicsSensitive = true;

    /**
     * Adjust the position and rotation of the shape (SFML) to fit the body (Box2D).
     */
    virtual void render(sf::RenderWindow *window) = 0;

    /**
     * Setter for entity type of the child inheriting this class.
     * @param id entity type to set the child to.
     */
    void setID(ID id) {
        Entity::id = id;
    };

    /**
     * Getter for entity type of the child inheriting this class.
     * @return the entity type of the child.
     */
    ID getID() const {
        return id;
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

    /// Contact Functions
    virtual void startContact(Entity *contactEntity) = 0;
    virtual void endContact(Entity *contactEntity) = 0;

protected:

    /// Important Entity Properties

    /**
     * The x- and y-position of the entity's origin in pixels.
     */
    float x{}, y{};

    ID id{};

    b2Body *body{};

    sf::Shape *sfShape{};

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
