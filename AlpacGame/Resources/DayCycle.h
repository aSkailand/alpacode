

#ifndef ALPACGAME_DAYCYCLE_H
#define ALPACGAME_DAYCYCLE_H

#include <iostream>
#include <Box2D/Box2D.h>
#include <chrono>

#include "ConfigGame.h"

class DayCycle {
private:
    float dayTime = 1.0f;



public:


    DayCycle(ConfigGame *configGame, float planetRadius, b2Body *body);

    sf::Shape *background = nullptr;



    void updateDayCycle();

    void render(sf::RenderWindow *window);

private:

    void loopDayCycle();
    ConfigGame *configGame = nullptr;

    sftools::Chronometer chrono;

    int bg;

    // Enums for picutre
    enum class backgroundCycle{MORNING_1, MORNING_2, MORNING_3,
                         AFTERNOON_4, AFTERNOON_5, AFTERNOON_6,
                         EVENING_7, EVENING_8, EVENING_9,
                         NIGHT_10, NIGHT_11, NIGHT_12};
    backgroundCycle currentBG;




};


#endif
