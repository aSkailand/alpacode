#ifndef ALPACGAME_TREES_H
#define ALPACGAME_TREES_H


#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class TreeHigh : public Scenery {
public:

    TreeHigh(ConfigGame *configGame, float width, float height,float angle);

    void render(sf::RenderWindow *window)  override;

    void update() override;

private:

    ConfigGame *configGame;

};


#endif //ALPACGAME_TREES_H
