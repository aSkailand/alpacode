#ifndef ALPACGAME_HEARTBAROMETER_H
#define ALPACGAME_HEARTBAROMETER_H

#include <iostream>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class ConfigGame;

class HitPointBarometer
{
public:

    HitPointBarometer(ConfigGame *configGame, int &HP, float width, float height);

    void setPlacement(float x, float y, float angle);

    void render(sf::RenderWindow *window);

private:

    /// Pointers
    ConfigGame *configGame = nullptr;

    sf::Text label_HP;

    int *HP = nullptr;

    sf::RectangleShape sfShape;
    sf::Texture Texture;
};


#endif //ALPACGAME_HEARTBAROMETER_H
