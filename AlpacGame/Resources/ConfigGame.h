
#ifndef ALPACGAME_CONFIGGAME_H
#define ALPACGAME_CONFIGGAME_H


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

/**
 * Common resources accessible by all game entities.
 */
class ConfigGame {
public:

    /**
     * The radius of the planet.
     */
    unsigned int planetRadius;

    /**
     * The position of the planet's center relatively to the game window.
     */
    sf::Vector2f planetCenter;

    /**
     * Run the given configurations.
     * @param window the shared window.
     */
    void run(sf::RenderWindow &window);

    /**
     * Offset on the placement of the planet.
     */
    // todo: make into static?

    /**
     * Calculates the x position on the circle's outline given by the angle.
     * @param angle the angle to calculate with.
     * @return The x position on the circle.
     */
    float calcX(float angle);

    /**
     * Calculates the y position on the circle's outline given by the angle.
     * @param angle the angle to calculate with.
     * @return The y position on the circle.
     */
    float calcY(float degree);

    /**
     * Calculates the x position given the angle and radius.
     * @param angle the angle to calculate with.
     * @param radius the radius to calculate with.
     * @return The x position.
     */
    float calcX(float degree, float radius);

    /**
     * Calculates the y position given the angle and radius.
     * @param angle the angle to calculate with.
     * @param radius the radius to calculate with.
     * @return The y position.
     */
    float calcY(float degree, float radius);

    // todo: Find a way to move this to gameState?
    /**
     * The change of time since last check.
     * Used to throttle down speed according to processor's power,
     * so that entity movements behave smoother.
     */
    float deltaTime;

    /**
     * The current buffered input, used to
     * determine which direction the farmer walks towards.
     */
    sf::Keyboard::Key currentInput;

};


#endif //ALPACGAME_CONFIGGAME_H
