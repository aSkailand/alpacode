
#ifndef ALPACGAME_CONFIGGAME_H
#define ALPACGAME_CONFIGGAME_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Box2D/Dynamics/b2Body.h>
#include <queue>

#include "../entity/Entity.h"
#include "../entity/EntityWarm.h"
#include "../Resources/SpriteInfo.h"
#include "../scenery/Scenery.h"
#include "../scenery/DayCycle/DayCycle.h"


/**
 * Common resources accessible by all game entities.
 */
class ConfigGame {
public:

    /// Day / Night Cycle
    enum class Cycle{DAY, NIGHT};
    Cycle getCurrentCycle() const;
    void setCurrentCycle(Cycle currentCycle);
    DayCycle *dayCycle = nullptr;
    unsigned int numOfDay = 1;

    unsigned int numOfAliveAlpacas = 0;

    /// Customizable Properties
    float sunRadius = 300.f;
    float planetRadius = 700.f;

    // todo: Necessary?
    Scenery* sun = nullptr;
    Scenery* sky = nullptr;

    bool newGame = true;
    bool isPaused = false;

    b2World* world = nullptr;
    Entity* planet = nullptr;
    Entity* farmer = nullptr;

    void reset();

    sf::RenderWindow *window = nullptr;

    std::vector<Entity*> *entities = nullptr;
    std::queue<b2Vec2> queue;

    sf::CircleShape mouseArrow;
    std::vector<Scenery*> *sceneries = nullptr;

    /**
     * Show in-game labels or not.
     */
    bool showDebugMode = false;

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

    //Create label
    sf::Text createLabel(sf::Font *font, unsigned int fontSize, const std::string &text);;

    /// Fonts
    sf::Font fontID;

    /// Textures

    // Single Textures
    sf::Texture healthTexture;

    sf::Texture fertileHeartTexture;

    sf::Texture trapOpenTexture;
    sf::Texture trapClosedTexture;

    sf::Texture shotgunHeldTexture;
    sf::Texture shotgunDropTexture;

    // Texture Vectors
    std::vector<sf::Texture> bulletIndicatorTextures;
    std::vector<sf::Texture> sunTextures;
    std::vector<sf::Texture> planetTextures;
    std::vector<sf::Texture> skyTextures;
    std::vector<sf::Texture*> cooldownTextures; // todo switch to non pointer

    // Texture Maps
    std::map<EntityWarm::Action, SpriteInfo> wolfSprites;
    std::map<EntityWarm::Action, SpriteInfo> alpacaSprites;
    std::map<EntityWarm::Action, SpriteInfo> farmerSprites;

private:

    Cycle currentCycle = Cycle::DAY;

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
