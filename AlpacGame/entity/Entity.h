
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
        PLANET = 0x0001,
        FARMER = 0x0002,
        ALPACA = 0x0004,
        WOLF = 0x0008
    };

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

    /**
     * The x- and y-position of the entity's origin in pixels.
     */
    float x{}, y{};

protected:

    ID id{};

    b2Body *body{};

    sf::Shape *sfShape{};

};

#endif //ALPACGAME_ENTITY_H
