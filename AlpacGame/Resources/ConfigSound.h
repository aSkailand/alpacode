#ifndef ALPACGAME_CONFIGSOUND_H
#define ALPACGAME_CONFIGSOUND_H

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Resources/ConfigMenu.h"

class ConfigSound {
public:

    enum class soundsID{SHOTGUN, STEPPING, JUMP};

    void run();

    bool isMenuMusicPlaying;

    bool isGameMusicPlaying;

    void playMenuMusic(bool playMusic);

    void playGameMusic(bool playMusic);

    void loadJumpSound();

    void setMasterVolume(int masterVolume);

    void setMusicVolume(int musicVolume);

    void setSoundEffects(int soundEffects);

    int getMasterVolume() const;

    int getMusicVolume() const;

    int getSoundEffects() const;

    std::map<soundsID, sf::Sound> mapSounds;

private:
    int masterVolume;
    int musicVolume;
    int soundEffects;

    sf::Music menuMusic;
    sf::Music gameMusic;

    sf::SoundBuffer tempStepBuffer;
    sf::SoundBuffer tempJumpBuffer;
    sf::SoundBuffer tempShotgunBuffer;

    sf::Sound tempJumpSound;
    sf::Sound tempStepSound;
    sf::Sound tempShotgunSound;


};


#endif //ALPACGAME_CONFIGSOUND_H
