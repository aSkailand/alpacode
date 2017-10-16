#include <random>
#include <chrono>
#include "WolfState.h"


void WolfState::goWolf(int elapsedTime) {

    int randomNumber;
    int direction;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);


    if (elapsedTime == tickSecond) {
        //randomNumber = rand() % 2 + 0;

        randomNumber = distribution(generator);

        std::cout << "The random number is: " << randomNumber << std::endl;
        //This happens every three second
        std::cout << "Tick" << std::endl;
        moveWolf = !moveWolf;
        std::cout << randomNumber << std::endl;
        if (randomNumber == 0) {
            currentActionState = WolfState::Action::IDLE;
            std::cout << "The wolf is idle..." << std::endl;
        } else if (randomNumber == 1) {
            currentActionState = WolfState::Action::WALKING;
            direction = distribution(generator);
            if (direction == 0) {
                currentDirectionState = WolfState::Direction::LEFT;
            } else if (direction == 1) {
                currentDirectionState = WolfState::Direction::RIGHT;
            }
        }
    }


    if (getCurrentActionState() == Action::WALKING) {
        if (currentDirectionState == WolfState::Direction::LEFT) {
            std::cout << "Left" << std::endl;
        } else if (currentDirectionState == WolfState::Direction::RIGHT) {
            std::cout << "Right" << std::endl;
        }
    }
}


void WolfState::setActionState(Action setAction) {

}

void WolfState::setDirectionState(Direction setDirection) {

}

int WolfState::getTickSecond() const {
    return tickSecond;
}

WolfState::Action WolfState::getCurrentActionState() {
    return currentActionState;
}

