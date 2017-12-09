
#include "DayCycle.h"
#include "../../Resources/ConfigGame.h"
#include "../../entity/planet/planet.h"
#include "../Sky/Sky.h"
#include "../Sun/Sun.h"

DayCycle::DayCycle(ConfigGame *configGame) {

    // Assign pointers
    this->configGame = configGame;
    this->planet = dynamic_cast<Planet *>(configGame->planet);
    this->sun = dynamic_cast<Sun *>(configGame->sun);
    this->sky = dynamic_cast<Sky *>(configGame->sky);

    // Assign texture containers
    sunTexture = configGame->sunTextures[0];
    moonTexture = configGame->sunTextures[2];

    // Set initial cycle
    configGame->setCurrentCycle(ConfigGame::Cycle::DAY);

    /// Calculations

    // Set current cycleFrame;
    cycleFrame = 0;

    // Calculate sun's distance
    planetToSunDistance = configGame->planetRadius + sunDistance;

    // Calculate new sun tick
    sunTick = (360.f / revolution) / 144.f;

    // Calculate time for one cycle
    cycleTime = revolution / 6.f;

    // Reset timer
    dayCycleTime.reset(false);

    std::cout << "DayCycle Initialized" << std::endl;

}

void DayCycle::proceed() {

    // Move sun
    updateSunMovement();

    // Check if it is time to proceed one tick
    if (dayCycleTime.getElapsedTime().asSeconds() >= cycleTime) {

        // Proceed one frame
        cycleFrame++;
        if (cycleFrame == 12) cycleFrame = 0;

        // Determine current cycle frame
        if (cycleFrame == 0) {
            configGame->setCurrentCycle(ConfigGame::Cycle::DAY);
            sun->setTexture(&sunTexture);
        } else if (cycleFrame == 6) {
            configGame->setCurrentCycle(ConfigGame::Cycle::NIGHT);
            sun->setTexture(&moonTexture);
        }

        // Switch textures
        planet->setTexture(cycleFrame);
        sky->setTexture(cycleFrame);

        // Reset cycle countdown time
        dayCycleTime.reset(true);
    }

}

void DayCycle::updateSunMovement() {

    float x = configGame->calcX(sunAngle, planetToSunDistance);
    float y = configGame->calcY(sunAngle, planetToSunDistance);

    sun->setPlacement(x, y, sunAngle);

    sunAngle += sunTick;
    if(sunAngle > 360.f) sunAngle = 0;
}


