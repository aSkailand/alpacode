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
    ConfigSound::masterVolume = 10 / (masterVolume + 1);
    if (masterVolume == 0) {
        ConfigSound::masterVolume = 100;
    }
}

void ConfigSound::setMusicVolume(int musicVolume) {
    ConfigSound::musicVolume = musicVolume;
    if (masterVolume == 0) {
        menuMusic.setVolume((musicVolume * 10));
        gameMusic.setVolume((musicVolume * 10));
    } else {
        menuMusic.setVolume((musicVolume * 10) / masterVolume);
        gameMusic.setVolume((musicVolume * 10) / masterVolume);
    }
}

void ConfigSound::setSoundEffects(int soundEffects) {
    this->soundEffects = soundEffects;
    if (masterVolume == 0) {

        mapSounds[ConfigSound::soundsID::JUMP].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::STEPPING].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::SHOTGUN].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::MORNING].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::NIGHT].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::OPEN_TRAP].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::CLOSE_TRAP].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::NO_AMMO].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::RELOAD].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::FARMER_HURT].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::WOLF_HURT].setVolume((soundEffects * 10));
        mapSounds[ConfigSound::soundsID::ALPACA_HURT].setVolume((soundEffects * 10));

    } else {

        mapSounds[ConfigSound::soundsID::JUMP].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::STEPPING].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::SHOTGUN].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::MORNING].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::NIGHT].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::OPEN_TRAP].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::CLOSE_TRAP].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::NO_AMMO].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::RELOAD].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::FARMER_HURT].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::WOLF_HURT].setVolume((soundEffects * 10) / masterVolume);
        mapSounds[ConfigSound::soundsID::ALPACA_HURT].setVolume((soundEffects * 10) / masterVolume);

    }
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

    tempMorningBuffer.loadFromFile("Resources/Sound/morning.wav");
    morningSound.setBuffer(tempMorningBuffer);
    mapSounds.emplace(soundsID::MORNING, morningSound);

    tempNightBuffer.loadFromFile("Resources/Sound/night.wav");
    nightSound.setBuffer(tempNightBuffer);
    mapSounds.emplace(soundsID::NIGHT, nightSound);

    tempOpenTrapBuffer.loadFromFile("Resources/Sound/openTrap.wav");
    openTrapSound.setBuffer(tempOpenTrapBuffer);
    mapSounds.emplace(soundsID::OPEN_TRAP, openTrapSound);

    tempCloseTrapBuffer.loadFromFile("Resources/Sound/closeTrap.wav");
    closeTrapSound.setBuffer(tempCloseTrapBuffer);
    mapSounds.emplace(soundsID::CLOSE_TRAP, closeTrapSound);

    tempNoAmmoBuffer.loadFromFile("Resources/Sound/noAmmo.wav");
    noAmmoSound.setBuffer(tempNoAmmoBuffer);
    mapSounds.emplace(soundsID::NO_AMMO, noAmmoSound);

    tempReloadBuffer.loadFromFile("Resources/Sound/reload.wav");
    reloadSound.setBuffer(tempReloadBuffer);
    mapSounds.emplace(soundsID::RELOAD, reloadSound);

    tempFarmerHurtBuffer.loadFromFile("Resources/Sound/farmerHurt.wav");
    farmerHurtSound.setBuffer(tempFarmerHurtBuffer);
    mapSounds.emplace(soundsID::FARMER_HURT, farmerHurtSound);

    tempWolfHurtBuffer.loadFromFile("Resources/Sound/wolfHurt.wav");
    wolfHurtSound.setBuffer(tempWolfHurtBuffer);
    mapSounds.emplace(soundsID::WOLF_HURT, wolfHurtSound);

    tempAlpacaHurtBuffer.loadFromFile("Resources/Sound/alpacaHurt.wav");
    alpacaHurtSound.setBuffer(tempAlpacaHurtBuffer);
    mapSounds.emplace(soundsID::ALPACA_HURT, alpacaHurtSound);

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