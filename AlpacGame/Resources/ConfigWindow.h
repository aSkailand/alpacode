
#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <SFML/Graphics.hpp>

class ConfigWindow {
public:
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    void run();

    sf::RenderWindow &getWindow();

private:
    sf::RenderWindow window;
    sf::VideoMode windowResolution;
};


#endif //ALPACGAME_CONFIG_H
