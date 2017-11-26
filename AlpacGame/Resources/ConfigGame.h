
#ifndef ALPACGAME_CONFIGGAME_H
#define ALPACGAME_CONFIGGAME_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Dynamics/b2Body.h>

#include "../entity/Entity.h"
#include "../entity/EntityWarm.h"
#include "../Resources/SpriteInfo.h"

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

    bool mouseInLeftSide = false;

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

    sf::Texture sun_1;
    sf::Texture sun_2;
    sf::Texture moon_1;


    sf::Texture shotgunHeldTexture;
    sf::Texture shotgunDropTexture;

    /// Map of every entities sprites.
    std::map<EntityWarm::Action, SpriteInfo> wolfSprites;
    std::map<EntityWarm::Action, SpriteInfo> alpacaSprites;
    std::map<EntityWarm::Action, SpriteInfo> farmerSpritesWithoutHands;
    std::map<EntityWarm::Action, SpriteInfo> farmerSpritesWithHands;


private:

    /**
    * Load all necessary font used during the game.
    */
    void loadAllFonts();

    /**
     * Load the given sprites into a vector, and then map it to the corresponding entity.
     * @param filename the path of the file containing the sprites.
     * @param spriteMap the corresponding map holding the sprites.
     * @param width the width of the sprite.
     * @param height the height of the sprite.
     * @param top //todo delete
     * @param action the key to associate the sprite with.
     */
    void loadTexture(std::string filename,
                     std::map<EntityWarm::Action, SpriteInfo> &spriteMap,
                     int width,
                     int height,
                     int top, EntityWarm::Action action);

    /**
     * Load all sprites and then map them in categorized fashion.
     */
    void loadAllTextures();

};


#endif //ALPACGAME_CONFIGGAME_H
