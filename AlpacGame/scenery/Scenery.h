
#ifndef ALPACGAME_SCENERY_H
#define ALPACGAME_SCENERY_H

#include <SFML/Graphics.hpp>

class Scenery {
public:

    virtual void render(sf::RenderWindow *window) = 0;
    virtual void update() = 0;

    sf::Shape *sf_Shape;

};

#endif //ALPACGAME_SCENERY_H
