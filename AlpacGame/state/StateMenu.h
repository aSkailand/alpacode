
#ifndef ALPACGAME_STATEMENU_H
#define ALPACGAME_STATEMENU_H

#include <iostream>
#include <TGUI/TGUI.hpp>
#include "State.h"
#include "StateMachine.h"

class StateMenu : public State {
public:

    /**
     * Draw menu and check for input before moving to next state.
     * @param stateMachine the StateMachine controlling this state.
     */
    void goNext(StateMachine &stateMachine);

private:
    // Pointers


    /**
     * Pointer to the one and only StateMachine.
     */
     StateMachine *machine;

    /**
     * Pointer to the one and only Window.
     */
    sf::RenderWindow *window;

    /**
     * Pointer to the one and only menuGUI
     */
    tgui::Gui *menuGUI;

    // Menu functions

    /**
     * Check if menu is still running.
     * @return true if no meaningful input is given, false if meaningful input is given.
     */
    bool pollMenu();

    /**
     * Draw the menu with all possible menu choices.
     */
    void drawMenu();

};


#endif //ALPACGAME_STATEMENU_H
