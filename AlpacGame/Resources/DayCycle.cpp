
#include "DayCycle.h"



DayCycle::DayCycle(ConfigGame *configGame, float planetRadius, b2Body *body) {

    this->configGame = configGame;

    configGame->currentCycle = ConfigGame::Cycle::DAY;
    
    float world_x = body->GetPosition().x * SCALE;
    float world_y = body->GetPosition().y * SCALE;

    background = new sf::CircleShape(2920+ planetRadius/2 );

    background->setOrigin(2920+ planetRadius/2, 2920+ planetRadius/2);
    background->setPosition(world_x, world_y);
    background->setTexture(&configGame->morning_1);
    background->setOutlineThickness(0);


    /// Iterate amove the pictures
    currentBG = backgroundCycle::MORNING_1;

    //Reset timer right away
    chrono.reset(true);
    std::cout << "DayCycle Initialized" << std::endl;
    
    
}

void DayCycle::loopDayCycle() {

    switch(currentBG){
        /// Day Starts
        case backgroundCycle ::MORNING_1:{
            background->setTexture(&configGame->morning_1);
            configGame->currentCycle = ConfigGame::Cycle::DAY;
            break;}
        case backgroundCycle ::MORNING_2:{      background->setTexture(&configGame->morning_2); break;}
        case backgroundCycle ::MORNING_3:{      background->setTexture(&configGame->morning_3); break;}
        case backgroundCycle ::AFTERNOON_4:{    background->setTexture(&configGame->afternoon_4); break;}
        case backgroundCycle ::AFTERNOON_5:{    background->setTexture(&configGame->afternoon_5); break;}
        case backgroundCycle ::AFTERNOON_6:{    background->setTexture(&configGame->afternoon_6); break;}
        case backgroundCycle ::EVENING_7:{      background->setTexture(&configGame->evening_7); break;}
        case backgroundCycle ::EVENING_8:{      background->setTexture(&configGame->evening_8); break;}
        /// Night Starts
        case backgroundCycle ::EVENING_9:{
            background->setTexture(&configGame->evening_9);
            configGame->currentCycle = ConfigGame::Cycle::NIGHT;
            break;}
        case backgroundCycle ::NIGHT_10:{      background->setTexture(&configGame->night_10); break;}
        case backgroundCycle ::NIGHT_11:{      background->setTexture(&configGame->night_11); break;}
        case backgroundCycle ::NIGHT_12:{      background->setTexture(&configGame->night_12); break;}
    }

}

void DayCycle::updateDayCycle(){



    if(chrono.getElapsedTime().asSeconds() >= dayTime){

        /// Day Time: dayTime = 5.s
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

        std::cout << chrono.getElapsedTime().asSeconds() << std::endl;
        chrono.reset(true);
    }



    /// Change Picture
    loopDayCycle();

}

void DayCycle::render(sf::RenderWindow *window) {

    window->draw(*background);



}
