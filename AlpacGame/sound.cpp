#include "sound.h"


SoundGame::SoundGame(StateMachine &stateMachine) {
    LoadSound();
    printf("Sound...");
}


void SoundGame::LoadSound() {
    if (!menuMusic.openFromFile("alpacaMusic.ogg")) {
        printf("Couldnt load music");
    }
    if (!walkingSound.openFromFile("Jump20.ogg")) {
        printf("Couldnt load sound");
    }
}

void SoundGame::StepSound() {

    walkingSound.play();
}

void SoundGame::PlayMusic() {
    menuMusic.play();
    menuMusic.setLoop(true);
    printf("Playing music... ");
}


