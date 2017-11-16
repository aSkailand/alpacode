#include <iostream>
#include "ConfigSound.h"

void ConfigSound::run() {
    masterVolume = 10;
    musicVolume = 10;
    soundEffects = 10;
}

void ConfigSound::setMasterVolume(int masterVolume) {
    ConfigSound::masterVolume = masterVolume;
    std::cout << masterVolume << std::endl;
}

void ConfigSound::setMusicVolume(int musicVolume) {
    ConfigSound::musicVolume = musicVolume;
    std::cout << musicVolume << std::endl;
}

void ConfigSound::setSoundEffects(int soundEffects) {
    ConfigSound::soundEffects = soundEffects;
    std::cout << soundEffects << std::endl;
}
