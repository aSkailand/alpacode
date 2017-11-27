

#ifndef ALPACGAME_DAYCYCLE_H
#define ALPACGAME_DAYCYCLE_H

#include <iostream>
#include <Box2D/Box2D.h>
#include <chrono>

#include "../../Resources/ConfigGame.h"
#include "../Scenery.h"

class DayCycle : public Scenery{

private:
    /// Time for each background transition
    float cycleTime = 10.0f;

    /// Radius for the Sun
    float sunRadius = 200.f;

    /// Sun starting point. Could fix this later on.
    float sunAngle = 210.f;

public:

    DayCycle(ConfigGame *configGame);

    sf::Shape *background = nullptr;
    sf::Shape *sun = nullptr;




private:

    void render(sf::RenderWindow *window) override;
    void update() override;

    void loopDayCycle();

    ConfigGame *configGame = nullptr;
    sftools::Chronometer cycleChrono;

    enum class backgroundCycle{MORNING_1, MORNING_2, MORNING_3,
                         AFTERNOON_4, AFTERNOON_5, AFTERNOON_6,
                         EVENING_7, EVENING_8, EVENING_9,
                         NIGHT_10, NIGHT_11, NIGHT_12};

    backgroundCycle currentBG;
    
    /// Overall movement including the sun and clouds.
    void updateSunMovement();

    /// The change in angle for the sun at each tick.
    float sunTick;
};


#endif
