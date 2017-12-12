
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

/**
 * DayCycle system handler:
 * Manage the day cycle aspects of the sky, sun and planet.
 * Queues up textures for the scenery and entities, and calculates sun movements.
 *
 * In short:
 *      1 whole day = 2 revolutions of the sun/moon.
 *      1 whole day = 12 cycle frames
 *      Therefore, 1 revolution is 6 cycle frames, which simply means that there
 *      will occur 6 transitions of cycles (and therefore textures) of the
 *      planet and sky.
 */
class DayCycle{

public:

    /// Public methods

    /**
     * Calculate initial values and clock.
     * @param configGame a pointer to shared resources.
     */
    explicit DayCycle (ConfigGame *configGame);

    /**
     * Proceed all the necessary updates per game tick.
     */
    void proceed();

    /**
     * Initiate clock, for public access.
     */
    void initiateClock(){
        dayCycleTime.reset(true);
    };

    void pause(){
        dayCycleTime.pause();
    };

    void resume(){
        dayCycleTime.resume();
    };


private:

    /// Pointers

    ConfigGame *configGame = nullptr;
    Planet *planet = nullptr;
    Sky *sky = nullptr;
    Sun *sun = nullptr;


    /// Local texture storage

    sf::Texture sunTexture;
    sf::Texture moonTexture;


    /// Time Variables

    // Current cycle (Max. 11)
    unsigned int cycleFrame;

    // One whole revolution in seconds
    float revolution = 15.f;

    // Time for each background transition (Calculated)
    float cycleTime;

    // Clock handling the time to proceed one cycle
    sftools::Chronometer dayCycleTime;


    /// Distance Variables

    // Distance of the sun from the planets surface.
    float sunDistance = 500.0f;

    // Distance from planet center to sun center (Calculated)
    float planetToSunDistance;


    /// Angle variables

    // Current angle
    float sunAngle = 0.f;

    // The angle to increment with for every world tick (Calculated)
    float sunTick;

    /**
     * Move sun around the planet by sun tick angle for each game loop tick.
     */
    void updateSunMovement();



};


#endif
