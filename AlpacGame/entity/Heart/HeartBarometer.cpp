#include "HeartBarometer.h"

HeartBarometer::HeartBarometer(sf::Texture texture, int HP, float width, float height)
{

    this->Texture = texture;

    sfShape.setSize(sf::Vector2f(width,height));
    sfShape.setOrigin(sfShape.getSize().x /2, sfShape.getSize().y /2);
    sfShape.setTexture(&Texture);
}

void HeartBarometer::setPlacement(float x, float y, float angle) {

    sfShape.setPosition(x,y);
    sfShape.setRotation(angle);
}

void HeartBarometer::render(sf::RenderWindow *window) {

    window->draw(sfShape);

}