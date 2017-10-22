
#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <SFML/Graphics.hpp>

class Config {
public:
    void run();

    sf::RenderWindow &getWindow();
    void setWindowResolution(int unsigned screenWidth, int unsigned screenHeight);

private:
    sf::RenderWindow window;
    sf::VideoMode windowResolution;
};


#endif //ALPACGAME_CONFIG_H
