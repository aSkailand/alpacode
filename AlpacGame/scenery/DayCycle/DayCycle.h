

#ifndef ALPACGAME_DAYCYCLE_H
#define ALPACGAME_DAYCYCLE_H

#include <iostream>
#include <Box2D/Box2D.h>
#include <chrono>

#include "../Scenery.h"
#include "../../Resources/ConfigGame.h"

class ConfigGame;
class DayCycle : public Scenery{

private:


    /// Time for each background transition
    float cycleTime = 5.0f;

    /// Distance of the sun from the planets surface.
    float sunDistance = 350.0f;
    /// Radius for the Sun
    float sunRadius = 250.f;

    /// Starting point
    float sunAngle = 210.f;

public:

    DayCycle (ConfigGame *configGame);

    sf::Shape *background = nullptr;
    sf::Shape *earth = nullptr;

    sf::Shape *sun = nullptr;

private:

    void render(sf::RenderWindow *window) override;
    void update() override;

    void loopDayCycle();

    ConfigGame *configGame = nullptr;
    sftools::Chronometer cycleChrono;

    /// Overall movement including the sun and clouds.
    void updateSunMovement();

    /// The change in angle for the sun at each tick.
    float sunTick;

    unsigned int counterBackground = 0;

    std::vector<sf::Texture *> bgCycle;
    std::vector<sf::Texture *> earthCycle;



};


#endif
