#include "WolfState.h"

void WolfState::goWolf(int elapsedTime) {
    int randomNumber;
    int direction;
    if (elapsedTime == tickSecond) {
        randomNumber = randomNumberGenerator();
        std::cout << randomNumber << std::endl;
        if (randomNumber == 0) {
            currentActionState = WolfState::Action::IDLE;
            std::cout << "The wolf is idle..." << std::endl;
        } else if (randomNumber == 1) {
            currentActionState = WolfState::Action::WALKING;
            direction = randomNumberGenerator();
            if (direction == 0) {
                currentDirectionState = WolfState::Direction::LEFT;
                std::cout << "The wolf is moving left!" << std::endl;
            } else if (direction == 1) {
                currentDirectionState = WolfState::Direction::RIGHT;
                std::cout << "The wolf is moving right!" << std::endl;
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

//Random number generator, gets a number between 0-1
int WolfState::randomNumberGenerator() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator);
}

int WolfState::getTickSecond() const {
    return tickSecond;
}

WolfState::Action WolfState::getCurrentActionState() {
    return currentActionState;
}

