#ifndef ALPACGAME_SUN_H
#define ALPACGAME_SUN_H


#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class Sun : public Scenery{
public:

    Sun(ConfigGame *configGame, float radius, float x, float y, float angle);

    void setPlacement(float x, float y, float angle);

    void render(sf::RenderWindow *window) override;

    void update() override;

    void setTexture(sf::Texture *texture);

private:

    ConfigGame *configGame;
};


#endif //ALPACGAME_SUN_H
