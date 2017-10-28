#ifndef ALPACGAME_SOUND_H
#define ALPACGAME_SOUND_H

#include "SFML/Audio.hpp"
#include "state/StateMachine.h"


class SoundGame {
public:

    SoundGame(StateMachine &stateMachine);

    void LoadSound();

    void PlayMusic();
    void StepSound();

    sf::Music menuMusic;

    sf::Music walkingSound;

};


#endif //ALPACGAME_SOUND_H
