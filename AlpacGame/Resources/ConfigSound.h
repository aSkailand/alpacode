#ifndef ALPACGAME_CONFIGSOUND_H
#define ALPACGAME_CONFIGSOUND_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ConfigSound {
public:

    bool isMenuMusicPlaying;

    bool isGameMusicPlaying;

    void playMenuMusic(bool playMusic);

    void playGameMusic(bool playMusic);

    void run();

    void setMasterVolume(int masterVolume);

    void setMusicVolume(int musicVolume);

    void setSoundEffects(int soundEffects);

private:

    int masterVolume;
    int musicVolume;
    int soundEffects;
    sf::Music menuMusic;
    sf::Music gameMusic;
};


#endif //ALPACGAME_CONFIGSOUND_H
