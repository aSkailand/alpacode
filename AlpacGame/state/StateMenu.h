#ifndef ALPACGAME_STATEMENU_H
#define ALPACGAME_STATEMENU_H

#include <iostream>
#include <TGUI/TGUI.hpp>
#include "State.h"
#include "StateMachine.h"

/**
 * This is the StateMenu class, it contains is the main menu for the game,
 * the main menu has three options, Play game, Settings and Quit.
 */
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

    void changeOption();

    // TGUI setup
    // Creates a theme
    tgui::Theme::Ptr theme = tgui::Theme::create("Resources/BabyBlue.txt");
    tgui::Layout windowWidth;
    tgui::Layout windowHeight;
    tgui::Picture::Ptr picture;
};


#endif //ALPACGAME_STATEMENU_H
