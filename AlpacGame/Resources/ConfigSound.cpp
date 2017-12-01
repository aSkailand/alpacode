#include <iostream>
#include "ConfigSound.h"

void ConfigSound::run() {

    isMenuMusicPlaying = false;
    isGameMusicPlaying = false;
    masterVolume = 10;
    musicVolume = 6;
    soundEffects = 8;

    menuMusic.setVolume(musicVolume * 10);
    gameMusic.setVolume(musicVolume * 10);

    loadJumpSound();

}

void ConfigSound::playMenuMusic(bool playMusic) {
    isGameMusicPlaying = false;
    gameMusic.stop();
    if (!menuMusic.openFromFile("Resources/Sound/menuMusic.wav")) {
        std::cout << "Couldnt load 'menuMusic.wav'" << std::endl;
    } else {
        std::cout << "Playing menu music" << std::endl;
    }

    isMenuMusicPlaying = playMusic;
    menuMusic.setLoop(true);
    menuMusic.play();
}

void ConfigSound::playGameMusic(bool playMusic) {
    isMenuMusicPlaying = false;
    menuMusic.stop();
    if (!gameMusic.openFromFile("Resources/Sound/gameMusic.wav")) {
        std::cout << "Couldnt load 'gameMusic.wav'" << std::endl;
    } else {
        std::cout << "Playing game music" << std::endl;
    }

    isGameMusicPlaying = playMusic;
    gameMusic.setLoop(true);
    gameMusic.play();
}


int ConfigSound::getMasterVolume() const {
    return masterVolume;
}

int ConfigSound::getMusicVolume() const {
    return musicVolume;
}

int ConfigSound::getSoundEffects() const {
    return soundEffects;
}

void ConfigSound::setMasterVolume(int masterVolume) {
    ConfigSound::masterVolume = masterVolume;

    std::cout << masterVolume << std::endl;
}

void ConfigSound::setMusicVolume(int musicVolume) {
    ConfigSound::musicVolume = musicVolume;
    menuMusic.setVolume(musicVolume * 10);
    gameMusic.setVolume(musicVolume * 10);
    std::cout << musicVolume << std::endl;
}

void ConfigSound::setSoundEffects(int soundEffects) {
    ConfigSound::soundEffects = soundEffects;
    std::cout << soundEffects << std::endl;
}

void ConfigSound::loadJumpSound() {

    tempJumpBuffer.loadFromFile("Resources/Sound/jump.wav");

    tempJumpSound.setBuffer(tempJumpBuffer);
    mapSounds.emplace(soundsID::JUMP,tempJumpSound);

    tempStepBuffer.loadFromFile("Resources/Sound/step.wav");

    tempStepSound.setBuffer(tempStepBuffer);
    mapSounds.emplace(soundsID::STEPPING, tempStepSound);

    tempShotgunBuffer.loadFromFile("Resources/Sound/shot.wav");

    tempShotgunSound.setBuffer(tempShotgunBuffer);
    mapSounds.emplace(soundsID::SHOTGUN, tempShotgunSound);


}



