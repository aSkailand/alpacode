
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
    this->planetTextures = configGame->planetTextures;
    this->skyTextures = configGame->skyTextures;

    // Set initial cycle
    configGame->setCurrentCycle(ConfigGame::Cycle::DAY);

    // Calculate sun's distance
    planetToSunDistance = configGame->planetRadius + sunDistance;

    // Calculate new sun tick
    float revolution = cycleTime * 6;
    sunTick = revolution / 360.f;

    //Reset timer right away
    dayCycleTime.reset(true);

    std::cout << "DayCycle Initialized" << std::endl;

}

void DayCycle::proceed() {

    // Check if it is time to proceed one tick
    if (dayCycleTime.getElapsedTime().asSeconds() >= cycleTime) {

        // Proceed one tick
        cycleTick = cycleTick == 12 ? 0 : cycleTick++;

        // Determine current cycle
        if (cycleTick == 0) {
            configGame->setCurrentCycle(ConfigGame::Cycle::DAY);
            sun->setTexture(&sunTexture);
        } else if (cycleTick == 5) {
            configGame->setCurrentCycle(ConfigGame::Cycle::NIGHT);
            sun->setTexture(&moonTexture);
        }

        // Switch textures
        planet->setTexture(&planetTextures[cycleTick]);
        sky->setTexture(&skyTextures[cycleTick]);

        // Reset cycle countdown time
        dayCycleTime.reset(true);
    }

    // Move sun
    updateSunMovement();

}

void DayCycle::updateSunMovement() {

    float x = configGame->calcX(sunAngle, planetToSunDistance);
    float y = configGame->calcY(sunAngle, planetToSunDistance);

    sun->setPlacement(x, y, sunAngle);

    sunAngle += sunTick;
}


