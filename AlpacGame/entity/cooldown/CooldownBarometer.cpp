
#include "CooldownBarometer.h"

CooldownBarometer::CooldownBarometer(std::vector<sf::Texture *> &textureVector, sftools::Chronometer *clock, float width, float height) {

    // Assign variables
    this->clock = clock;
    this->textures = textureVector;

    // Calculate max amount of ticks needed to present barometer
    maxTicks = static_cast<unsigned int>(textures.size());

    // Create shape
    sfShape.setSize(sf::Vector2f(width, height));
    sfShape.setOrigin(sfShape.getSize().x / 2, sfShape.getSize().y / 2);
    sfShape.setTexture(textures.at(0));
//    sfShape.setOutlineThickness(2);

}

void CooldownBarometer::setPlacement(float x, float y, float angle) {

    if (clock->isRunning()) {
        sfShape.setPosition(x, y);
        sfShape.setRotation(angle);
    }

}

void CooldownBarometer::render(sf::RenderWindow *window) {

    // Stop render if clock is paused
    if (!clock->isRunning())
        return;

    // Stop proceeding counter if counter has reached end
    if (counter != maxTicks) {

        // Switch to correct texture
        if (clock->getElapsedTime().asSeconds() >= ticks[counter]) {
            sfShape.setTexture(textures.at(counter));
            counter++;
        }

    }

    // Render
    window->draw(sfShape);

}

void CooldownBarometer::calcTicks(float tick) {

    // Reset Barometer
    counter = 0;
    sfShape.setTexture(textures.at(0));

    // Calculate the delta between two adjacent ticks
    float tickDelta = tick / (float) maxTicks;

    // Reset tick container
    ticks.clear();
    for (int i = 0; i < maxTicks; ++i) {
        ticks.push_back(tickDelta * i);
    }
}
