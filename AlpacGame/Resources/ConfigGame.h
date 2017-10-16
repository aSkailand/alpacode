
#ifndef ALPACGAME_CONFIGGAME_H
#define ALPACGAME_CONFIGGAME_H


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class ConfigGame {
public:
    unsigned int planetRadius;
    sf::Vector2f planetCenter;
    int offset = 100;

    float planetRotation;

    // todo: make into static
    float calcX(float degree);
    float calcY(float degree);

    void run(sf::RenderWindow &window);

};


#endif //ALPACGAME_CONFIGGAME_H
