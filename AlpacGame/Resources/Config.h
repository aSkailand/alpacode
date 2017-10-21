
#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <SFML/Graphics.hpp>

class Config {
public:
    void run();

    sf::RenderWindow &getWindow();

    int unsigned screenWidth = 800;
    int unsigned screenHeight = 600;

private:
    sf::RenderWindow window;
    sf::VideoMode windowResolution;
};


#endif //ALPACGAME_CONFIG_H
