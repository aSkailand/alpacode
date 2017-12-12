
#ifndef ALPACGAME_CAVE_H
#define ALPACGAME_CAVE_H


#include <random>
#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class Cave : public Scenery{
public:
    Cave(ConfigGame *configGame, float width, float height);
    void reposition(float angle);
    void render(sf::RenderWindow *window) override;
    void update() override;

private:
    ConfigGame *configGame = nullptr;


};


#endif //ALPACGAME_CAVE_H
