#ifndef ALPACGAME_TREEMEDIUM_H
#define ALPACGAME_TREEMEDIUM_H


#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class TreeMedium : public Scenery {

public:
    TreeMedium(ConfigGame *configGame, float width, float height,float angle);

    void render(sf::RenderWindow *window) override ;

    void update() override ;

private:

    ConfigGame *configGame;
};


#endif //ALPACGAME_TREEMEDIUM_H
