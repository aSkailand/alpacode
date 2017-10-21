#ifndef ALPACGAME_STATEOPTION_H
#define ALPACGAME_STATEOPTION_H

#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"

class StateOption : public State {

public:

    void goNext(StateMachine &stateMachine);


private:
    // Pointers
    StateMachine *machine;
    sf::RenderWindow *window;


    //Option Functions
    /**
     * Check if option is still running.
     */
    bool PollOption();

    /**
     * Draw option menu with all possible option choices.
     */
    void drawOption();

    /**
     * Generate all option choices.
     */
    void initOptionStrings();

    /**
     * Wrapper for adding new option choices.
     * @param choice name of option choice.
     * @param x the x-position to draw the added option choice.
     * @param y the y-position to draw the added option choice.
     */
    void addOptionChoice(const std::string &choice, int x, int y);

    void moveUp();
    void moveDown();

    void lowRes();
    void highRes();


    //Sets font of texts in option
    sf::Font fontOptionChoice;

    const int unsigned fontSizeOptionChoice = 50;
    const sf::Color colorSelected = sf::Color::Red;
    const sf::Color colorDeselected = sf::Color::Green;

    int optionIndex = 0;
    std::vector<sf::Text> optionChoices;
};


#endif //ALPACGAME_STATEOPTION_H
