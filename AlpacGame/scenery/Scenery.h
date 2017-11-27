
#ifndef ALPACGAME_SCENERY_H
#define ALPACGAME_SCENERY_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Scenery {
public:

    virtual void render(sf::RenderWindow *window) = 0;
    virtual void update() = 0;

};

#endif //ALPACGAME_SCENERY_H
