#ifndef ALPACGAME_STATEOPTION_H
#define ALPACGAME_STATEOPTION_H

#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"


/**
 * The StateOption class contains four different menus, the main option menu
 * which contains video setting, control settings and sound settings.
 * The user can configure the settings for the game to better fit their needs.
 */
class StateOption : public State {

public:

    void goNext(StateMachine &stateMachine);

    /**
     * The video settings menu,
     * how it looks and how it functions
     */
    void videoSettings();


private:

    // Pointers
    StateMachine *machine;
    sf::RenderWindow *window;
    tgui::Gui *menuGUI;

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
     * This is the
     */
   // void menuSettings();


    /**
     * The control settings menu,
     * how it looks and how it functions
     */
    void controlSettings();

    /**
     * The suond settings menus,
     * how it looks and how it functions
     */
    void soundSettings();

    /**
     * This function applies all the changes the user have made in the different menus
     */
    void applyChanges();

    /**
     * Sets the windows resolution to 800x600
     */
    void lowRes();

    /**
     * Sets the  windows resolution to 1280x720
     */
    void medRes();

    /**
     * Sets the windows resolution to 1920x1080
     */
    void highRes();

    /**
     * Temporary resolution string
     * @param tempRes, a string that is later used to change the windowresolution
     */
    void setTempRes(const std::string &tempRes);

    /**
     * Takes in a string "800x600", "1280x720" and "1920x1080" and changes resolution accordingly
     * @param res, the string that sets the resoulution
     */
    void setResoulution(std::string res);

    // TGUI setup
    tgui::Picture::Ptr pictureMenu;
    tgui::Button::Ptr masterButton;
    tgui::VerticalLayout::Ptr layout;
    tgui::CheckBox::Ptr fullScreenCheck;
    tgui::Layout windowWidth;
    tgui::Layout windowHeight;
    // Creates a theme
    tgui::Theme::Ptr theme = tgui::Theme::create("Resources/BabyBlue.txt");


    // Variables
    std::string tempRes = "800x600";
    int unsigned tempXres = 800;
    int unsigned tempYres = 600;
    int initialVolumeLevel = 10;
};

#endif //ALPACGAME_STATEOPTION_H
