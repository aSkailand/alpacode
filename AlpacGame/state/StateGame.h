
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
#include "../entity/CollisionListener.h"

#include "../entity/planet/planet.h"
#include "../entity/player/Farmer.h"

#include "../entity/alpaca/Alpaca.h"
#include "../entity/wolf/Wolf.h"
#include "../entity/shotgun/Shotgun.h"


class StateGame : public State{
public:

    void goNext(StateMachine &stateMachine) override;

    std::vector<Entity*> *entities;

    Entity* planet;
    Farmer* farmer;




private:

    sf::CircleShape *background;

    /// Pointers
    StateMachine *machine;
    ConfigGame *configGame;
    sf::RenderWindow *window;

    /// Box2D Variables
    b2World* world;
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    /// View
    sf::View view;
    float viewNonZoomed = 1.5f;  // How much to zoom in
    float viewZoomed = 4.0f;
    bool zoomed = false;

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
