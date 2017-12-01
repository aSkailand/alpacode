#include <iostream>
#include "ConfigSound.h"

void ConfigSound::run() {

    isMenuMusicPlaying = false;
    isGameMusicPlaying = false;

    // Initiate default volume levels
    masterVolume = 10;
    musicVolume = 6;
    soundEffects = 8;

    // Sets the default volume
    menuMusic.setVolume(musicVolume * 10);
    gameMusic.setVolume(musicVolume * 10);

    // Loads all the different sounds
    loadSounds();

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

void ConfigSound::setMasterVolume(int masterVolume) {
    ConfigSound::masterVolume = masterVolume;
}

void ConfigSound::setMusicVolume(int musicVolume) {
    ConfigSound::musicVolume = musicVolume;
    menuMusic.setVolume(musicVolume * 10);
    gameMusic.setVolume(musicVolume * 10);
}

void ConfigSound::setSoundEffects(int soundEffects) {
    ConfigSound::soundEffects = soundEffects;
    mapSounds[ConfigSound::soundsID::JUMP].setVolume(soundEffects * 10);
    mapSounds[ConfigSound::soundsID::STEPPING].setVolume(soundEffects * 10);
    mapSounds[ConfigSound::soundsID::SHOTGUN].setVolume(soundEffects * 10);

}

void ConfigSound::loadSounds() {

    tempJumpBuffer.loadFromFile("Resources/Sound/jump.wav");

    jumpSound.setBuffer(tempJumpBuffer);
    mapSounds.emplace(soundsID::JUMP, jumpSound);

    tempStepBuffer.loadFromFile("Resources/Sound/step.wav");

    stepSound.setBuffer(tempStepBuffer);
    mapSounds.emplace(soundsID::STEPPING, stepSound);

    tempShotgunBuffer.loadFromFile("Resources/Sound/shot.wav");

    shotgunSound.setBuffer(tempShotgunBuffer);
    mapSounds.emplace(soundsID::SHOTGUN, shotgunSound);
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