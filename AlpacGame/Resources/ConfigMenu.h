#ifndef ALPACGAME_CONFIGMENU_H
#define ALPACGAME_CONFIGMENU_H

#include "map"
#include "SFML/Graphics.hpp"
#include "ConfigWindow.h"
#include <TGUI/TGUI.hpp>

class StateMachine;
class ConfigWindow;

/**
 * This class creates and holds all of the layouts and widgets of the menu GUI.
 */
class ConfigMenu {
public:

    StateMachine *machine = nullptr;

    enum class layouts {
        MAINMENU,
        SETTINGS,
        VIDEO,
        SOUND,
        CONTROLS
    };

    enum class buttons {
        PLAYGAME,
        SETTINGS,
        QUIT,
        VIDEO,
        SOUND,
        CONTROLS,
        BACK_TO_MAIN,
        APPLY_VIDEO_SETTINGS,
        APPLY_SOUND_SETTINGS,
        MASTER_SLIDER,
        MUSIC_SLIDER,
        EFFECT_SLIDER
    };

    /**
     * Getter for the current RESOLUTION enum class
     * @return returns the current resolution
     */
    ConfigWindow::Resolution getCurrentResolution() const;

    /**
     * Sets the current resolution
     * @param currentResolution
     */
    void setCurrentResolution(ConfigWindow::Resolution currentResolution);

    /// Mappings of the different layouts and widgets
    std::map<buttons, tgui::Button::Ptr> mapButtons;
    std::map<layouts, tgui::VerticalLayout::Ptr> mapLayouts;
    std::map<buttons, tgui::Slider::Ptr> mapSliders;

    /**
     * Getter for the background picture, this image is only loaded once to save resources
     * @return returns a tgui::Picture
     */
    tgui::Picture::Ptr &getPictureMenu();

    void run(StateMachine &stateMachine);

    /**
     * Creates a button
     * @param buttonType
     * @param buttonName
     * @param typeActivation how the button is activated, E.g. pressed, released
     * @param func binds a function to the button, this function runs when the button is pressed
     */
    void createButton(
            buttons buttonType,
            const std::string &buttonName,
            const std::string &typeActivation,
            const std::function<void()> &func
    );

    /**
     * Creates a slider
     * @param sliderType enum class, the key value to the <map> the slider is emplaced in
     */
    void createSlider(buttons sliderType);

    /**
     * Creates the main menu layout and settings menu layout
     * @param Width from the SFML window
     */
    void mainMenuLayout(tgui::Gui *Width);

    /**
     * Creates the video settings layout
     */
    void videoSettingsLayout();

    /**
     * Creates the sound settings layout
     */
    void soundSettingsLayout();

    bool checkBoxMuteChecked = false;

    bool checkBoxFullScreenChecked = false;

private:
//    resolution currentResolution = resolution::RES800x600;

    tgui::VerticalLayout::Ptr tempVerticalLayout;
    tgui::Picture::Ptr pictureMenu;
    tgui::Button::Ptr masterButton;
    tgui::Theme::Ptr theme;
    tgui::Layout windowWidth;
    tgui::Layout windowHeight;

};


#endif //ALPACGAME_CONFIGMENU_H
