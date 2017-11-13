#ifndef ALPACGAME_CONFIGMENU_H
#define ALPACGAME_CONFIGMENU_H

#include "map"
#include "SFML/Graphics.hpp"
#include <TGUI/TGUI.hpp>

class ConfigMenu {
public:

    enum class MAP_LAYOUTS {
        MAINMENU, SETTINGS, VIDEO, SOUND, CONTROLS

    };

    enum class MAP_BUTTONS {
        PLAYGAME, SETTINGS, QUIT, VIDEO, SOUND, CONTROLS, BACK_TO_MAIN, COMBOBOX, FULLSCREEN, APPLY
    };

    std::map<MAP_BUTTONS, tgui::Button::Ptr> mapButtons;
    std::map<MAP_LAYOUTS, tgui::VerticalLayout::Ptr> mapLayouts;

    tgui::Picture::Ptr &getPictureMenu();


    void run();

    /**
     * Creates a button
     * @param buttonType
     * @param buttonName
     * @param typeActivation how the button is activated, E.g. pressed, released
     * @param func binds a function to the button, this function runs when the button is pressed
     */
    void createButton(
            MAP_BUTTONS buttonType,
            std::string buttonName,
            std::string typeActivation,
            std::function<void()> func
    );
    /**
     * Creates the main menu layout and settings menu layout
     * @param Width from the SFML window
     */
    void createLayouts(tgui::Gui *Width);

    /**
     * Creates the video settings layout
     */
    void videoSettingsLayout();

    /**
     * Creates the sound settings layout
     */
    void soundSettingsLayout();

private:
    tgui::VerticalLayout::Ptr tempVerticalLayout;
    tgui::Picture::Ptr pictureMenu;
    tgui::Button::Ptr masterButton;
    tgui::Theme::Ptr theme = tgui::Theme::create("Resources/BabyBlue.txt");
    tgui::Layout windowWidth;
    tgui::Layout windowHeight;
};



#endif //ALPACGAME_CONFIGMENU_H
