
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
     * Pointer to the one and only Gui.
     */
    tgui::Gui *gui;

    // Menu functions

    /**
     * Generate all menu choices.
     */
    void initMenuStrings();

    /**
     * Check if menu is still running.
     * @return true if no meaningful input is given, false if meaningful input is given.
     */
    bool pollMenu();

    /**
     * Draw the menu with all possible menu choices.
     */
    void drawMenu();


    // Menu Choice Objects

    /**
     * Current selected menu choice.
     */
    int menuIndex = 0;

    /**
     * Vector containing all possible menu choices.
     */
    std::vector<sf::Text> menuChoices;  // todo: Make this one initialize once only


    // Menu Choice functions

    /**
     * Wrapper for adding new menu choices.
     * @param choice name of menu choice.
     * @param x the x-position to draw the added menu choice.
     * @param y the y-position to draw the added menu choice.
     */
    void addMenuChoice(const std::string &choice, int x, int y);

    /**
     * Change selection accordingly if user switch selection upwards.
     */
    void moveUp();

    /**
     * Change selection accordingly if user switch selection downwards.
     */
    void moveDown();


    // Menu Choice Cosmetics

    sf::Font fontMenuChoice;
    const unsigned int fontSizeMenuChoice = 50;
    const sf::Color colorSelected = sf::Color::Red;
    const sf::Color colorDeselected = sf::Color::Green;
};


#endif //ALPACGAME_STATEMENU_H
