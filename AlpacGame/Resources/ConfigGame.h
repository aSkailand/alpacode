
#ifndef ALPACGAME_CONFIGGAME_H
#define ALPACGAME_CONFIGGAME_H


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Dynamics/b2Body.h>

/**
 * Common resources accessible by all game entities.
 */
class ConfigGame {
public:

    /// Day / Night Cycle
    enum class Cycle{DAY, NIGHT};
    Cycle currentCycle;


    /// Customizable Properties
    /**
     * The radius of the planet.
     */
    unsigned int planetRadius = 600;

    b2Body* planetBody;


    /**
     * Show in-game labels or not.
     */
    bool showLabels = false;

    /**
     * The position of the planet's center relatively to the game window.
     */
    sf::Vector2f planetCenter;

    /**
     * Run the given configurations.
     * @param window the shared window.
     */
    void run(sf::RenderWindow &window);

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

    /**
     * The current buffered input, used to
     * determine which direction the farmer walks towards.
     */
    sf::Keyboard::Key currentInput = sf::Keyboard::Unknown;

    /// Fonts
    sf::Font fontID;

    /// Textures
    sf::Texture planetTexture;
    sf::Texture farmerTexture;
    sf::Texture alpacaTexture;
    sf::Texture wolfTexture;
    sf::Texture alertRedTexture;
    sf::Texture alertYelTexture;

    sf::Texture morning_1;
    sf::Texture morning_2;
    sf::Texture morning_3;
    sf::Texture afternoon_4;
    sf::Texture afternoon_5;
    sf::Texture afternoon_6;
    sf::Texture evening_7;
    sf::Texture evening_8;
    sf::Texture evening_9;
    sf::Texture night_10;
    sf::Texture night_11;
    sf::Texture night_12;




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
