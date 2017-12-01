#include "HeartBarometer.h"

HeartBarometer::HeartBarometer(sf::Texture texture, int HP, float width, float height)
{

    this->Texture = texture;

    sfShape.setSize(sf::Vector2f(width,height));
    sfShape.setOrigin(sfShape.getSize().x /2, sfShape.getSize().y /2);
    sfShape.setTexture(&Texture);
}

void HeartBarometer::render(sf::RenderWindow *window) {

    sfShape.
}