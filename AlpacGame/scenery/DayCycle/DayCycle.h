
#ifndef ALPACGAME_DAYCYCLE_H
#define ALPACGAME_DAYCYCLE_H

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "SFML/Chronometer.hpp"

/*
 * Forward declarations. Used here to avoid deep recursions of includes,
 * more specific from ConfigGame and its contents.
 */
class ConfigGame;
class Planet;
class Sun;
class Sky;


class DayCycle{

public:

    explicit DayCycle (ConfigGame *configGame);

    void proceed();

private:

    ConfigGame *configGame = nullptr;
    Planet *planet = nullptr;
    Sky *sky = nullptr;
    Sun *sun = nullptr;

    std::vector<sf::Texture> planetTextures;
    std::vector<sf::Texture> skyTextures;
    sf::Texture sunTexture;
    sf::Texture moonTexture;


    /// Time for each background transition
    float cycleTime = 5.0f;

    /// Distance of the sun from the planets surface.
    float sunDistance = 350.0f;

    // Distance from planet center to sun center
    float planetToSunDistance;

    /// Starting point
    float sunAngle = 0.f;

    sftools::Chronometer dayCycleTime;

    /// Overall movement including the sun and clouds.
    void updateSunMovement();

    /// The change in angle for the sun at each tick.
    float sunTick;

    unsigned int cycleTick = 0;



};


#endif
