
#include "DayCycle.h"

// TODO: IMPORTANT Fix Background Size and Origin with different PlanetSize. :(


DayCycle::DayCycle(ConfigGame *configGame) {

    this->configGame = configGame;

    configGame->setCurrentCycle(ConfigGame::Cycle::DAY);
    
    float world_x = configGame->planetBody->GetWorldCenter().x * SCALE;
    float world_y = configGame->planetBody->GetWorldCenter().y * SCALE;

    /// Intialize Background
    background = new sf::CircleShape(1920+ configGame->planetRadius/2 );
    background->setOrigin(1920+ configGame->planetRadius/2, 1920+ configGame->planetRadius/2);
    background->setPosition(world_x, world_y);
    background->setTexture(&configGame->morning_1);

    /// Initialize Sun
    sun = new sf::CircleShape(sunRadius);
    sun->setOrigin(sunRadius, sunRadius);
    sun->setTexture(&configGame->sun_1);

    /// Iterate amove the pictures
    currentBG = backgroundCycle::MORNING_1;

    //Reset timer right away
    cycleChrono.reset(true);
    std::cout << "DayCycle Initialized" << std::endl;

    /** Calculation for the suns movement
     *  1.f / 23.f is for the sun to circulate the world in 1 second
     *  multipy by the amount of seconds in the cycle.
     *  divide be 2 for the equal amount of sun and night
     */
    float dayTime = cycleTime * 12;
    sunTick = 1.f / ((23.f * dayTime) / 2);

    
    
}

void DayCycle::loopDayCycle() {

    switch(currentBG){
        /// Day Starts
        case backgroundCycle ::MORNING_1:{
            background->setTexture(&configGame->morning_1);
            sun->setTexture(&configGame->sun_1);
            configGame->setCurrentCycle(ConfigGame::Cycle::DAY);
            break;}
        case backgroundCycle ::MORNING_2:{      background->setTexture(&configGame->morning_2); break;}
        case backgroundCycle ::MORNING_3:{      background->setTexture(&configGame->morning_3); break;}
        case backgroundCycle ::AFTERNOON_4:{    background->setTexture(&configGame->afternoon_4);   sun->setTexture(&configGame->sun_2);   break;}
        case backgroundCycle ::AFTERNOON_5:{    background->setTexture(&configGame->afternoon_5); break;}
        case backgroundCycle ::AFTERNOON_6:{    background->setTexture(&configGame->afternoon_6); break;}

            /// Night Starts TODO:
        case backgroundCycle ::EVENING_7:{
            background->setTexture(&configGame->evening_7);
            sun->setTexture(&configGame->moon_1);
            configGame->setCurrentCycle(ConfigGame::Cycle::NIGHT);
            break;}
        case backgroundCycle ::EVENING_8:{      background->setTexture(&configGame->evening_8); break;}
        case backgroundCycle ::EVENING_9:{     background->setTexture(&configGame->evening_9); break;}
        case backgroundCycle ::NIGHT_10:{      background->setTexture(&configGame->night_10); break;}
        case backgroundCycle ::NIGHT_11:{      background->setTexture(&configGame->night_11); break;}
        case backgroundCycle ::NIGHT_12:{      background->setTexture(&configGame->night_12); break;}
    }

}

void DayCycle::update(){
    updateSunMovement();


    if(cycleChrono.getElapsedTime().asSeconds() >= cycleTime){

        if(currentBG == backgroundCycle::MORNING_1)         currentBG = backgroundCycle ::MORNING_2;
        else if(currentBG == backgroundCycle::MORNING_2)    currentBG = backgroundCycle ::MORNING_3;
        else if(currentBG == backgroundCycle::MORNING_3)    currentBG = backgroundCycle ::AFTERNOON_4;
        else if(currentBG == backgroundCycle::AFTERNOON_4)  currentBG = backgroundCycle ::AFTERNOON_5;
        else if(currentBG == backgroundCycle::AFTERNOON_5)  currentBG = backgroundCycle ::AFTERNOON_6;
        else if(currentBG == backgroundCycle::AFTERNOON_6)  currentBG = backgroundCycle ::EVENING_7;
        else if(currentBG == backgroundCycle::EVENING_7)    currentBG = backgroundCycle ::EVENING_8;
        else if (currentBG == backgroundCycle::EVENING_8)   currentBG = backgroundCycle ::EVENING_9;
        else if(currentBG == backgroundCycle::EVENING_9)    currentBG = backgroundCycle ::NIGHT_10;
        else if(currentBG == backgroundCycle::NIGHT_10)     currentBG = backgroundCycle ::NIGHT_11;
        else if(currentBG == backgroundCycle::NIGHT_11)     currentBG = backgroundCycle ::NIGHT_12;
        else if (currentBG == backgroundCycle::NIGHT_12)    currentBG = backgroundCycle ::MORNING_1;

      //  std::cout << cycleChrono.getElapsedTime().asSeconds() << std::endl;
        cycleChrono.reset(true);
    }



    /// Change Picture
    loopDayCycle();

}

void DayCycle::updateSunMovement() {

    float x = configGame->planetBody->GetWorldCenter().x *SCALE + cos(sunAngle) * configGame->planetRadius * 2;
    float y = configGame->planetBody->GetWorldCenter().y *SCALE + sin(sunAngle) * configGame->planetRadius * 2;

    sun->setPosition(x,y);
    sunAngle += sunTick;
}

void DayCycle::render(sf::RenderWindow *window) {

    window->draw(*background);
    window->draw(*sun);

}


