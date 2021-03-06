
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
#include "../scenery/DayCycle/DayCycle.h"
#include "../scenery/Scenery.h"
#include "../entity/CollisionListener.h"

#include "../entity/planet/planet.h"
#include "../entity/player/Farmer.h"

#include "../entity/alpaca/Alpaca.h"
#include "../entity/wolf/Wolf.h"
#include "../entity/shotgun/Shotgun.h"
#include "../entity/trap/Trap.h"


class StateGame : public State{
public:
    bool testDefeat = false;
    void goNext(StateMachine &stateMachine) override;

    std::vector<Entity*> *entities;

    Entity* planet;
    Farmer* farmer;

    // Accessing DayCycle variables
    DayCycle *dayCycle;

private:

    /// Pointers
    StateMachine *machine;
    ConfigGame *configGame;
    ConfigSound *configSound;

    sf::RenderWindow *window;
    tgui::Gui *menuGUI;

    /// Box2D Variables
    b2World* world;
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    /// View
    sf::View view;
    float viewNonZoomed = 1.5f;  // How much to zoom in
    float viewZoomed = 3.0f;
    bool zoomed = false;

    // Game State Functions
    /**
     * Check if game is ongoing.
     * @return true if game is still running, false if an call of exit is done.
     */
    bool pollGame();

    void mousePressedHandler(sf::Event event);

    void keyPressedHandler(sf::Event event);

    /**
     * Checks if the player is dead or there is less than two alpacas
     * if this is true, there will pop up a defeat screen.
     * the player enters a name and the score will be saved in the higscore
     */
    void checkDefeat();
};


#endif //ALPACGAME_STATEGAME_H
