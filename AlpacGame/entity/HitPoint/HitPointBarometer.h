#ifndef ALPACGAME_HEARTBAROMETER_H
#define ALPACGAME_HEARTBAROMETER_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>


class ConfigGame;

class HitPointBarometer
{
public:

    HitPointBarometer(ConfigGame *configGame,sf::Texture texture, std::string startHP, float width, float height);

    void setPlacement(float x, float y, float angle);

    void render(sf::RenderWindow *window, std::string HP);

private:

    /// Pointers
    ConfigGame *configGame = nullptr;

    sf::Text label_HP;

    std::string HP;

    sf::RectangleShape sfShape;
    sf::Texture Texture;
};


#endif //ALPACGAME_HEARTBAROMETER_H
