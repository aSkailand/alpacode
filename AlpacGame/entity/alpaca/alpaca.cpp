#include <chrono>
#include "alpaca.h"

Alpaca::Alpaca(float x, float y, const Config& config, sf::RenderWindow& window)
        : x(x),
          y(y),
          config(config),
          window(window)

{
    rectangle.setSize(sf::Vector2f(size, size));
    rectangle.setFillColor(sf::Color::Red);
    this->currentDirection = currentDirection;
    this->currentAction = currentAction;
}

void Alpaca::move(float delta)
{
    Action::WALKING;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        x += speed * delta;
        Direction::RIGHT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        x -= speed * delta;
        Direction::LEFT;
    }

    // Keep the box within screen borders
    x = std::max(x, 0.f);
    x = std::min(x, (float)(config.windowWidth - size));
    y = std::max(y, 0.f);
    y = std::min(y, (float)(config.windowHeight - size));

    Action::IDLE;
}

void Alpaca::draw()
{
    // Position the rectangle used to draw the square
    rectangle.setPosition(x, y);

    // Draw the square
    window.draw(rectangle);
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

Alpaca::Direction Alpaca::getCurrentDirection() const {
    return currentDirection;
}

Alpaca::Action Alpaca::getCurrentAction() const {
    return currentAction;
}
/*enum class State { STATE_LOGIN };

State s = State::STATE_LOGIN;

std::cout << (int)s;

int randomTall = 1;
s = (State)randomTall;*/
