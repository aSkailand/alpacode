#ifndef ALPACGAME_STATEOPTION_H
#define ALPACGAME_STATEOPTION_H

#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"
#include "../Resources/ConfigGame.h"

/**
 * The StateOption class contains four different menus, the main option menu
 * which contains video setting, control settings and sound settings.
 * The user can configure the settings for the game to better fit their needs.
 */
class StateOption : public State {
public:
    void goNext(StateMachine &stateMachine);

private:
    // Pointers
    StateMachine *machine;
    sf::RenderWindow *window;
    tgui::Gui *menuGUI;
    ConfigGame *configGame;
    ConfigMenu *configMenu;


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
     * Key binding a new key and edit text to buttons.
     */
    void KeyBinding(sf::Keyboard::Key newKey, std::string keyString);

    void checkIfAnyControlAssigningKeyIsPressed(sf::Event event);

    /**
     * Checks if all the buttons got their default binds
     * and if they do, disables the default button
     */
    void DefaultKeysCheck();

};

#endif //ALPACGAME_STATEOPTION_H
