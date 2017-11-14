#ifndef ALPACGAME_CONFIGSOUND_H
#define ALPACGAME_CONFIGSOUND_H


class ConfigSound {
public:
    void run();

    void setMasterVolume(int masterVolume);

    void setMusicVolume(int musicVolume);

    void setSoundEffects(int soundEffects);

private:

    int masterVolume;
    int musicVolume;
    int soundEffects;
};


#endif //ALPACGAME_CONFIGSOUND_H
