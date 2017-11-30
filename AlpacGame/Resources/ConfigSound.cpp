#include <iostream>
#include "ConfigSound.h"

void ConfigSound::run() {
    isMenuMusicPlaying = false;
    isGameMusicPlaying = false;
    masterVolume = 10;
    musicVolume = 6;
    soundEffects = 8;

    menuMusic.setVolume(musicVolume*10);
    gameMusic.setVolume(musicVolume*10);
}

void ConfigSound::setMasterVolume(int masterVolume) {
    ConfigSound::masterVolume = masterVolume;
    std::cout << masterVolume << std::endl;
}

void ConfigSound::setMusicVolume(int musicVolume) {
    ConfigSound::musicVolume = musicVolume;
    menuMusic.setVolume(musicVolume*10);
    gameMusic.setVolume(musicVolume*10);
    std::cout << musicVolume << std::endl;
}

void ConfigSound::setSoundEffects(int soundEffects) {
    ConfigSound::soundEffects = soundEffects;
    std::cout << soundEffects << std::endl;
}

void ConfigSound::playMenuMusic(bool playMusic) {
    isGameMusicPlaying = false;
    gameMusic.stop();
    if (!menuMusic.openFromFile("Resources/menuMusic.wav")) {
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
    if (!gameMusic.openFromFile("Resources/gameMusic.wav")) {
        std::cout << "Couldnt load 'gameMusic.wav'" << std::endl;
    } else {
        std::cout << "Playing game music" << std::endl;
    }

    isGameMusicPlaying = playMusic;
    gameMusic.setLoop(true);
    gameMusic.play();
}

void ConfigSound::shotgunSound() {
    shotgunBuffer.loadFromFile("Resources/shot.wav");
    shotgun.setBuffer(shotgunBuffer);
    shotgun.play();
}

