
#ifndef ALPACGAME_STATEGAME_H
#define ALPACGAME_STATEGAME_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#define ENUM_TO_STR(ENUM) std::string(#ENUM)
#include "State.h"
#include "StateMachine.h"
#include "../../entity/alpaca/alpaca.h"


class StateGame : public State{
public:
    void goNext(StateMachine &stateMachine);
private:

    // Pointers
    StateMachine *machine;
    sf::RenderWindow *window;

    Config config;

    sf::Clock clock;
    sf::Time currentTime;


    // Game State Functions
    /**
     * Check if game is ongoing.
     * @return true if game is still running, false if an call of exit is done.
     */
    bool pollGame();

};


#endif //ALPACGAME_STATEGAME_H
