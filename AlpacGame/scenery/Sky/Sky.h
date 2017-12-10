//
// Created by Trong on 07/12/2017.
//

#ifndef ALPACGAME_SKY_H
#define ALPACGAME_SKY_H


#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class Sky : public Scenery{
public:

    sf::CircleShape shapeForeground;
    sf::CircleShape shapeBackground;

    Sky(ConfigGame *configGame, float radius, float x, float y);

    void render(sf::RenderWindow *window) override;

    void update() override;

    void setTexture(unsigned int cycleFrame);

private:

    ConfigGame *configGame = nullptr;
};


#endif //ALPACGAME_SKY_H
