#ifndef ALPACGAME_STONE_H
#define ALPACGAME_STONE_H


#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class Stone :  public Scenery{

public:
    Stone(ConfigGame *configGame, float width, float height, float angle);

    void render(sf::RenderWindow *window) override;

    void update() override;

private:

    ConfigGame *configGame;
};


#endif //ALPACGAME_STONE_H
