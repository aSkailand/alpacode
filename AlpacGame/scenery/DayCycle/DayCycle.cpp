#include <vector>

#include "DayCycle.h"


// TODO: IMPORTANT Fix Background Size and Origin with different PlanetSize. :(


DayCycle::DayCycle(ConfigGame *configGame) {

    this->configGame = configGame;

    configGame->setCurrentCycle(ConfigGame::Cycle::DAY);
    
    float world_x = configGame->planetBody->GetWorldCenter().x * SCALE;
    float world_y = configGame->planetBody->GetWorldCenter().y * SCALE;

    /// Initialize Background
    background = new sf::CircleShape(3000);
    background->setOrigin(background->getRadius(), background->getRadius());
    background->setPosition(world_x, world_y);
    background->setTexture(&configGame->morning_1);

    earth = new sf::CircleShape(configGame->planetRadius+70);
    earth->setOrigin(earth->getRadius(), earth->getRadius());
    earth->setPosition(world_x, world_y);
    earth->setTexture(&configGame->earth_1);

    /// Initialize Sun
    sun = new sf::CircleShape(sunRadius);
    sun->setOrigin(sun->getRadius(), sun->getRadius());
    sun->setTexture(&configGame->sun_1);

    /// Iterate amove the picture



    /** Calculation for the suns movement
     *  1.f / 23.f is for the sun to circulate the world in 1 second
     *  multipy by the amount of seconds in the cycle.
     *  divide be 2 for the equal amount of sun and night
     */


    bgCycle.push_back(&configGame->morning_1);
    bgCycle.push_back(&configGame->morning_2);
    bgCycle.push_back(&configGame->morning_3);
    bgCycle.push_back(&configGame->afternoon_4);
    bgCycle.push_back(&configGame->afternoon_5);
    bgCycle.push_back(&configGame->afternoon_6);
    bgCycle.push_back(&configGame->evening_7);
    bgCycle.push_back(&configGame->evening_8);
    bgCycle.push_back(&configGame->evening_9);
    bgCycle.push_back(&configGame->night_10);
    bgCycle.push_back(&configGame->night_11);
    bgCycle.push_back(&configGame->night_12);

    earthCycle.push_back(&configGame->earth_1);
    earthCycle.push_back(&configGame->earth_2);
    earthCycle.push_back(&configGame->earth_3);
    earthCycle.push_back(&configGame->earth_4);
    earthCycle.push_back(&configGame->earth_5);
    earthCycle.push_back(&configGame->earth_6);
    earthCycle.push_back(&configGame->earth_7);
    earthCycle.push_back(&configGame->earth_8);
    earthCycle.push_back(&configGame->earth_9);
    earthCycle.push_back(&configGame->earth_10);
    earthCycle.push_back(&configGame->earth_11);
    earthCycle.push_back(&configGame->earth_12);

    float dayTime = 30.f;
//    sunTick = 1.f / ((23.f * dayTime) / 2);
    sunTick = dayTime / 360.f;

    //Reset timer right away
    cycleChrono.reset(true);
    std::cout << "DayCycle Initialized" << std::endl;
    
}

void DayCycle::update(){

//    if(cycleChrono.getElapsedTime().asSeconds() >= 5.0f){
//        printf("sunAngleAAA: %f\n", sunAngle);
//    }

    if(sunAngle >= 60.f){
        printf("Time: %f\n", cycleChrono.getElapsedTime().asSeconds());
        printf("sunAngleAAA: %f\n", sunAngle);
    }

    updateSunMovement();

    if(cycleChrono.getElapsedTime().asSeconds() >= cycleTime){

//        printf("\tSun Angle: %f\n", sunAngle);

        counterBackground++;
        if(counterBackground == 12 ) counterBackground = 0;

        // Day
        if(counterBackground == 0){
            configGame->setCurrentCycle(ConfigGame::Cycle::DAY);
            sun->setTexture(&configGame->sun_1);
        }

        else if (counterBackground == 5){
            configGame->setCurrentCycle(ConfigGame::Cycle::NIGHT);
            sun->setTexture(&configGame->moon_1);
        }

        earth->setTexture(earthCycle.at(counterBackground));
        background->setTexture(bgCycle.at(counterBackground));
        cycleChrono.reset(true);
    }


    /// Change Picture

}

void DayCycle::updateSunMovement() {

//    float x = configGame->planetBody->GetWorldCenter().x * SCALE + cos(sunAngle) * (configGame->planetRadius + sunDistance);
//    float y = configGame->planetBody->GetWorldCenter().y * SCALE + sin(sunAngle) * (configGame->planetRadius + sunDistance);

//    printf("Seconds: %f\n", cycleChrono.getElapsedTime().asSeconds());

//    if(sunAngle >= 60.f){
//        printf("sunAngleAAA: %f\n", sunAngle);
//    }

    float x = configGame->calcX(sunAngle, configGame->planetRadius + sunDistance);
    float y = configGame->calcY(sunAngle, configGame->planetRadius + sunDistance);

    sun->setPosition(x,y);
    sunAngle += sunTick;
}

void DayCycle::render(sf::RenderWindow *window) {

    window->draw(*background);
    window->draw(*earth);
    window->draw(*sun);

}


