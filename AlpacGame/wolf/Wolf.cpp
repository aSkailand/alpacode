#include "Wolf.h"


Wolf::Wolf(sf::RenderWindow &renderWindow) {

    window = &renderWindow;
    windowSize = sf::VideoMode(window->getSize().x, window->getSize().y);

    // loads a texutre
    if(!wolfTexture.loadFromFile("wolf/wolf.png")){
        std::cout << "Error loading file!" << std::endl;
    }

    currentActionState = Action::IDLE;
    currentDirectionState = Direction::RIGHT;

    wolfRectangle = sf::RectangleShape(sf::Vector2f(size,size));
    wolfRectangle.setTexture(&wolfTexture);
    wolfRectangle.setOrigin(wolfRectangle.getSize().x / 2, wolfRectangle.getSize().y);
    wolfRectangle.setPosition(100,100);


}

void Wolf::draw(){
    window->draw(wolfRectangle);
}

void Wolf::control(float rotationDelta) {
    elapsedTime = clock.getElapsedTime();
    if((int)clock.getElapsedTime().asSeconds() == getTickSecond()){
        clock.restart();
    }

    // calculate positions

    x = windowSize.width / 2 + ()

    int direction;
    if ((int)elapsedTime.asSeconds() == tickSecond) {
        int randomNumber = randomNumberGenerator(0, 1);
        std::cout << randomNumber << std::endl;
        switch (randomNumber) {
            case (int) Wolf::Action::IDLE:
                //Do nothing
                std::cout << "The wolf is idle..." << std::endl;
                break;
            case (int) Wolf::Action::WALKING:
                direction = randomNumberGenerator(0, 1);
                std::cout << direction << std::endl;
                switch (direction) {
                    case (int) Wolf::Direction::LEFT:
                        std::cout << "The wolf is moving left!" << std::endl;
                        break;
                    case (int) Wolf::Direction::RIGHT:
                        std::cout << "The wolf is moving right!" << std::endl;
                        break;
                    default:break;
                }
                break;

            default:break;
        }
    }
    if (getCurrentActionState() == Action::WALKING) {
        if (currentDirectionState == Wolf::Direction::LEFT) {

        } else if (currentDirectionState == Wolf::Direction::RIGHT) {

        }
    }
}

/**
 * This takes in two integers, one lower and one upper and generates a random number
 * between.
 * @param lower
 * @param upper
 * @return returns a random number between lower and upper
 */
int Wolf::randomNumberGenerator(int lower, int upper) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}

int Wolf::getTickSecond() const {
    return tickSecond;
}

Wolf::Action Wolf::getCurrentActionState() {
    return currentActionState;
}

