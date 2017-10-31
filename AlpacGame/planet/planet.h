
#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../state/StateGame.h"

#include "../entity/Entity.h"

class Planet : public Entity{
public:

    Planet(b2World *world, float radius, float x, float y);

    void adjust() override;

    void switchAction() override;

private:

    void loadTextures();

    sf::RenderWindow *window;

    sf::Sprite backgroundSprite;
    sf::Texture planetTexture;
    sf::Texture backgroundTexture;
};

#endif