
#ifndef ALPACGAME_STATEMENU_H
#define ALPACGAME_STATEMENU_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"

class StateMenu : public State{
public:
    void goNext(StateMachine &stateMachine);

    sf::Text menu[3];   // todo: make this into vector

    void initMenuStrings();

    // todo: simplify these two into 1 function?
    void moveUp();
    void moveDown();

    int GetPressedItem() { return menuIndex; }

private:
    sf::Font font;

    int menuIndex = 0;
};


#endif //ALPACGAME_STATEMENU_H
