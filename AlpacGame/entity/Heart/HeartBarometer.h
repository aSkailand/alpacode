#ifndef ALPACGAME_HEARTBAROMETER_H
#define ALPACGAME_HEARTBAROMETER_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class HeartBarometer {
public:

    HeartBarometer(sf::Texture texture, int HP, float width, float height);

    void render(sf::RenderWindow *window);

private:

    sf::RectangleShape sfShape;
    sf::Texture Texture;
};


#endif //ALPACGAME_HEARTBAROMETER_H
