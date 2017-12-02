
#ifndef ALPACGAME_COOLDOWNBAROMETER_H
#define ALPACGAME_COOLDOWNBAROMETER_H

#include <SFML/Chronometer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../../Resources/ConfigGame.h"

class CooldownBarometer {
public:

    /**
     * CONSTRUCTOR: Create a visual barometer which is controlled by the given clock.
     * @param textureVector the vector of texture the barometer will use as its look.
     * @param clock the clock the barometer will follow.
     */
    CooldownBarometer(std::vector<sf::Texture *> &textureVector, sftools::Chronometer *clock,
                      float width, float height);

    /**
     * Update the position and rotation of the sfml shape of the barometer.
     * @param x the new x position.
     * @param y the new y position.
     * @param angle the new angle in degrees.
     */
    void setPlacement(float x, float y, float angle);

    /**
     * Render the barometer to the given window.
     * @param window the window to render the barometer on.
     */
    void render(sf::RenderWindow *window);

    /**
     * Restart the barometer, and recalculate all the lesser ticks.
     * Used to change the pace of the switching of textures.
     * @param tick the tick to end on.
     */
    void calcTicks(float tick);

private:

    unsigned int maxTicks = 0;
    unsigned int counter = 0;

    sf::RectangleShape sfShape;
    sftools::Chronometer *clock = nullptr;
    std::vector<sf::Texture*> textures;
    std::vector<float> ticks;
};


#endif //ALPACGAME_COOLDOWNBAROMETER_H
