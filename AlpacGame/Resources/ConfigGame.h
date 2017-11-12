
#ifndef ALPACGAME_CONFIGGAME_H
#define ALPACGAME_CONFIGGAME_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Dynamics/b2Body.h>
#include "../entity/Entity.h"

/**
 * Common resources accessible by all game entities.
 */
class ConfigGame {
public:

    /// Customizable Properties
    /**
     * The radius of the planet.
     */
    unsigned int planetRadius = 600;

    bool newGame = true;

    b2World* world = nullptr;
    Entity* planet = nullptr;
    Entity* farmer = nullptr;
    b2Body* planetBody = nullptr;

    void reset();

    sf::RenderWindow *window = nullptr;

    std::vector<Entity*> *entities = nullptr;

    /**
     * Show in-game labels or not.
     */
    bool showLabels = false;

    /**
     * The position of the planet's center relatively to the game window.
     */
    sf::Vector2f planetCenter;

    /**
     * The current buffered input, used to
     * determine which direction the farmer walks towards.
     */
    sf::Keyboard::Key currentInput = sf::Keyboard::Unknown;

    /**
     * Mouse Coordinates.
     */
    float mouseXpos = 0;
    float mouseYpos = 0;

    /**
     * Run the given configurations.
     * @param window the shared window.
     */
    void run(sf::RenderWindow &window);

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

    /// Fonts
    sf::Font fontID;

    /// Textures
    sf::Texture planetTexture;
    sf::Texture farmerTexture;
    sf::Texture alpacaTexture;
    sf::Texture wolfTexture;
    sf::Texture shotgunTexture;

private:

    /**
    * Load all necessary font used during the game.
    */
    void loadAllFonts();

    /**
     * Load all necessary textures used during the game.
     */
    void loadAllTextures();
};


#endif //ALPACGAME_CONFIGGAME_H
