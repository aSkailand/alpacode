#ifndef ALPACGAME_STATEOPTION_H
#define ALPACGAME_STATEOPTION_H

#include <SFML/Graphics.hpp>

#include "State.h"
#include "StateMachine.h"

class StateOption : public State {

public:


    // Creates a theme
    tgui::Theme::Ptr theme = tgui::Theme::create("C:/dev/libs/TGUI/include/TGUI/widgets/BabyBlue.txt");


    enum settingsID {
        SETTINGS, VIDEO, CONTROLS, SOUND
    };

    void goNext(StateMachine &stateMachine);


private:

    settingsID currentSettings;

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

    void menuSettings();

    void videoSettings();

    void controlSettings();

    void soundSettings();

    void applyChanges();

    void lowRes();

    void medRes();

    void highRes();

    void setTempRes(const std::string &tempRes);

    void setResoulution(std::string res);

    tgui::Picture::Ptr picture;

    tgui::Button::Ptr masterButton;

    tgui::VerticalLayout::Ptr layout;

    tgui::CheckBox::Ptr fullScreenCheck;


    std::string tempRes;
    int unsigned tempXres;
    int unsigned tempYres;

    int initialVolumeLevel;

    tgui::Layout windowWidth;
    tgui::Layout windowHeight;


};


#endif //ALPACGAME_STATEOPTION_H
