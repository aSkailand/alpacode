#include "Wolf.h"


Wolf::Wolf(StateMachine &stateMachine, float initAngle) {

    // Assigning pointers
    configGame = &stateMachine.configGame;
    window = &stateMachine.configWindow.getWindow();

    // Loads a texutre
    if (!wolfTexture.loadFromFile("entity/wolf/wolfy.png")) {
        std::cout << "Error loading file!" << std::endl;
    }

    // Assigning default states
    currentActionState = Action::IDLE;
    currentDirectionState = Direction::RIGHT;

    // Define the wolf
    wolfRectangle = sf::RectangleShape(sf::Vector2f(size, size));
    wolfSprite.setTexture(wolfTexture);
    wolfSprite.setScale(sf::Vector2f(0.5f, 0.5f));
    wolfSprite.setOrigin(wolfRectangle.getSize().x / 2, wolfRectangle.getSize().y + spriteOffset);

    // Assigning initial position
    angle = initAngle;
}

// The class that sets the wolfs position and rotation and draws the wolf
void Wolf::draw() {
    wolfSprite.setRotation(angle);
    wolfSprite.setPosition(x, y);
    window->draw(wolfSprite);
}

void Wolf::control() {

    int randomNumber;
    int randomDirection;

    // The clock that restarts every thrid second
    elapsedTime = clock.getElapsedTime();
    if ((int) clock.getElapsedTime().asSeconds() == getTickSecond()) {
        clock.restart();
    }

    // Position calculation
    x = configGame->calcX(angle);
    y = configGame->calcY(angle);

    /* The wolfs random movement control, the randomNumberGenerator(a, b) generates a number
     * from 0 - 1 randomly, if the random number is 0 the wolf does nothing (IDLE) and if the
     * random number is 1 the wolf moves (WALKING) left(0) or right(1)
     */
    if ((int) elapsedTime.asSeconds() == tickSecond) {
        randomNumber = randomNumberGenerator(0, 1);
        switch (randomNumber) {
            case (int) Wolf::Action::IDLE:
                currentActionState = Wolf::Action::IDLE;
                std::cout << wolfID << " The wolf is idle..." << std::endl;
                break;
            case (int) Wolf::Action::WALKING:
                currentActionState = Wolf::Action::WALKING;
                randomDirection = randomNumberGenerator(0, 1);
                switch (randomDirection) {
                    case (int) Wolf::Direction::LEFT:
                        std::cout << wolfID << " The wolf is moving left!" << std::endl;
                        wolfSprite.setScale({-0.5f, 0.5f});
                        currentDirectionState = Wolf::Direction::LEFT;
                        break;
                    case (int) Wolf::Direction::RIGHT:
                        std::cout << wolfID << " The wolf is moving right!" << std::endl;
                        wolfSprite.setScale({0.5f, 0.5f});
                        currentDirectionState = Wolf::Direction::RIGHT;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    // Moves the wolf
    if (currentActionState == Action::WALKING) {
        if (currentDirectionState == Wolf::Direction::LEFT) {
            angle += configGame->deltaTime * speed;
        } else if (currentDirectionState == Wolf::Direction::RIGHT) {
            angle -= configGame->deltaTime * speed;

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
