
#ifndef ALPACGAME_PLANET_H
#define ALPACGAME_PLANET_H

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../state/StateGame.h"

#include "../entity/EntityCold.h"

class Planet : public EntityCold{
public:

    Planet(b2World *world, float radius, float x, float y);

    void adjust() override;

private:

    void loadTextures();

    sf::RenderWindow *window;

    sf::Sprite backgroundSprite;
    sf::Texture planetTexture;
    sf::Texture backgroundTexture;
};

#endif