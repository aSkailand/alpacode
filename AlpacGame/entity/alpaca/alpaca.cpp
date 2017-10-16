#include <chrono>
#include "alpaca.h"

Alpaca::Alpaca(sf::RenderWindow &renderWindow) {
//    rectangle.setSize(sf::Vector2f(size, size));
//    rectangle.setFillColor(sf::Color::Red);
//    this->currentDirection = currentDirection;
//    this->currentAction = currentAction;

    window = &renderWindow;

    if (!alpacaTexture.loadFromFile("planet/alpaca.png")) {
        std::cout << "Error!!!" << std::endl;
    }

    // Define moving alpaca
    alpaca = sf::RectangleShape(sf::Vector2f(150, 150));
    alpaca.setTexture(&alpacaTexture);
    alpaca.setOrigin(alpaca.getSize().x / 2, alpaca.getSize().y);
    alpaca.rotate(90);
    alpaca.setOutlineThickness(1);

    // Change enums
    currentDirection = Direction::RIGHT;
    currentAction = Action::IDLE;

    // Clock
    currentTime = clock.restart();
}

void Alpaca::move()
{
//
//    currentTime = clock.getElapsedTime();
//    int cT = (int)clock.getElapsedTime().asSeconds();

    if (actionTick < clock.getElapsedTime().asSeconds()){
        randomAction();
        clock.restart();
    }

//    // Position the rectangle used to draw the square
//    rectangle.setPosition(x, y);

//    Action::WALKING;
//
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//        x += speed * delta;
//        Direction::RIGHT;
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//        x -= speed * delta;
//        Direction::LEFT;
//    }
//
//    // Keep the box within screen borders
//    x = std::max(x, 0.f);
//    x = std::min(x, (float)(config.windowWidth - size));
//    y = std::max(y, 0.f);
//    y = std::min(y, (float)(config.windowHeight - size));
//
//    Action::IDLE;
}

void Alpaca::draw()
{


    // Draw the square
    window->draw(alpaca);
}


void Alpaca::randomAction(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0,1);
    int random = distribution(generator);

    currentAction = (Action)random;

    if (currentAction == Action::IDLE){
        std::cout << "Alpaca is now in IDLE" << std::endl;
    }
    else if (currentAction == Action::WALKING){

        std::cout << "Alpaca is now WALKING" << std::endl;
        random = distribution(generator);
        currentDirection = (Direction)random;

        if (currentDirection == Direction::RIGHT){
            std::cout << "Alpaca is going to the RIGHT" << std::endl;
        }
        else if (currentDirection == Direction::LEFT){
            std::cout << "Alpaca is going to the LEFT" << std::endl;
        }

    }
}
