
#ifndef ALPACGAME_STATEGAME_H
#define ALPACGAME_STATEGAME_H

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "State.h"
#include "StateMachine.h"
#include "../Resources/ConfigWindow.h"
#include "../Resources/ConfigGame.h"
#include "../planet/planet.h"
#include "../entity/player/Farmer.h"
#include "../entity/alpaca/Alpaca.h"
#include "../entity/wolf/Wolf.h"

class StateGame : public State{
public:

    void goNext(StateMachine &stateMachine) override;

    std::vector<Entity*> entities;

    // todo: Put planet and farmer in vector, then point to them with these pointers?
    Entity* planet;
    Entity* farmer;

private:

    /// Pointers
    StateMachine *machine;
    ConfigGame *configGame;
    sf::RenderWindow *window;

    /// Box2D Variables
    b2World* world;
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // View
    sf::View view;
    float viewZoom = 1.0f;  // How much to zoom in
    int viewOffset = 400;   // How much is the view's offset

    // Game State Functions
    /**
     * Check if game is ongoing.
     * @return true if game is still running, false if an call of exit is done.
     */
    bool pollGame();

    void mousePressedHandler(sf::Event event);

    void keyPressedHandler(sf::Event event);
};


#endif //ALPACGAME_STATEGAME_H
