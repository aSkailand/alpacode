//
// Created by Bruker on 09/10/2017.
//

#ifndef ALPACGAME_APP_H
#define ALPACGAME_APP_H


#include <SFML/Graphics/RenderWindow.hpp>

#include "config.h"
#include "square.h"

class App
{
public:
    App();
    bool Tick();

protected:
    Config config;
    sf::RenderWindow window;
    Square square;

    sf::Clock clock;
};


#endif //ALPACGAME_APP_H
