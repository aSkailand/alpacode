#ifndef ALPACGAME_TREELOW_H
#define ALPACGAME_TREELOW_H


#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class TreeLow : public Scenery {
public:

    TreeLow(ConfigGame *configGame, float width, float height, float angle);

    void render(sf::RenderWindow *window) override ;

    void update() override ;

private:

    ConfigGame *configGame;

};


#endif //ALPACGAME_TREELOW_H
