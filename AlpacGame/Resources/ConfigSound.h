#ifndef ALPACGAME_CONFIGSOUND_H
#define ALPACGAME_CONFIGSOUND_H

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Resources/ConfigMenu.h"

class ConfigSound {
public:

    enum class soundsID {
        SHOTGUN, STEPPING, JUMP, MORNING, NIGHT, OPEN_TRAP, CLOSE_TRAP, NO_AMMO, RELOAD, FARMER_HURT, WOLF_HURT, ALPACA_HURT
    };

    /**
     * Defines the ConfigSound class, sets the default volume and creates
     * the different sounds
     **/

    void run();

    bool isMenuMusicPlaying;

    bool isGameMusicPlaying;

    /**
     * Loads and Plays the menu music when in the Menu or Option state
     * @param playMusic checks if the music is already playing so
     * that the music does not play over each other
     */
    void playMenuMusic(bool playMusic);

    /**
     * Loads and Plays the in game music when in the Singleplayer state
     * @param playMusic checks if the music is already playing so
     * that the music does not play over each other
     */
    void playGameMusic(bool playMusic);

    /**
     * Loads all the different sounds and saves them in a map
     */
    void loadSounds();

    /// Setters for different volume

    void setMasterVolume(int masterVolume);

    void setMusicVolume(int musicVolume);

    void setSoundEffects(int soundEffects);

    /// Getter for different volume

    int getMasterVolume() const;

    int getMusicVolume() const;

    int getSoundEffects() const;

    std::map<soundsID, sf::Sound> mapSounds;

private:

    /// All volume variables
    int masterVolume;
    int musicVolume;
    int soundEffects;

    /**
    * Sound, SoundBuffers and Music
    * used for loading and playing sound files
    */

    sf::Music menuMusic;
    sf::Music gameMusic;

    sf::SoundBuffer tempStepBuffer;
    sf::SoundBuffer tempJumpBuffer;
    sf::SoundBuffer tempShotgunBuffer;
    sf::SoundBuffer tempMorningBuffer;
    sf::SoundBuffer tempNightBuffer;
    sf::SoundBuffer tempOpenTrapBuffer;
    sf::SoundBuffer tempCloseTrapBuffer;
    sf::SoundBuffer tempNoAmmoBuffer;
    sf::SoundBuffer tempReloadBuffer;
    sf::SoundBuffer tempFarmerHurtBuffer;
    sf::SoundBuffer tempWolfHurtBuffer;
    sf::SoundBuffer tempAlpacaHurtBuffer;

    sf::Sound jumpSound;
    sf::Sound stepSound;
    sf::Sound shotgunSound;
    sf::Sound morningSound;
    sf::Sound nightSound;
    sf::Sound openTrapSound;
    sf::Sound closeTrapSound;
    sf::Sound noAmmoSound;
    sf::Sound reloadSound;
    sf::Sound farmerHurtSound;
    sf::Sound wolfHurtSound;
    sf::Sound alpacaHurtSound;

};


#endif //ALPACGAME_CONFIGSOUND_H
