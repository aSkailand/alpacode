#ifndef ALPACGAME_CONFIGMENU_H
#define ALPACGAME_CONFIGMENU_H

#include "map"
#include "SFML/Graphics.hpp"
#include <TGUI/TGUI.hpp>

/**
 * This class creates and holds all of the layouts and widgets of the menu GUI
 *
 */
class ConfigMenu {
public:

    enum class MAP_LAYOUTS {
        MAINMENU,
        SETTINGS,
        VIDEO,
        SOUND,
        CONTROLS
    };

    enum class MAP_BUTTONS {
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


    enum class RESOLUTION {
        RES800x600,
        RES1280x720,
        RES1920x1080
    };

    /**
     * Getter for the current RESOLUTION enum class
     * @return returns the current resolution
     */
    RESOLUTION getCurrentResolution() const;

    /**
     * Sets the current resolution
     * @param currentResolution
     */
    void setCurrentResolution(RESOLUTION currentResolution);

    /// Mappings of the different layouts and widgets
    std::map<MAP_BUTTONS, tgui::Button::Ptr> mapButtons;
    std::map<MAP_LAYOUTS, tgui::VerticalLayout::Ptr> mapLayouts;
    std::map<MAP_BUTTONS, tgui::Slider::Ptr> mapSliders;

    /**
     * Getter for the background picture, this image is only loaded once to save resources
     * @return returns a tgui::Picture
     */
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
     * Creates a slider
     * @param sliderType enum class, the key value to the <map> the slider is emplaced in
     */
    void createSlider(MAP_BUTTONS sliderType);

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

    bool checkBoxFullscreenChecked = false;

private:
    RESOLUTION currentResolution;
    tgui::VerticalLayout::Ptr tempVerticalLayout;
    tgui::Picture::Ptr pictureMenu;
    tgui::Button::Ptr masterButton;
    tgui::Theme::Ptr theme;
    tgui::Layout windowWidth;
    tgui::Layout windowHeight;

};


#endif //ALPACGAME_CONFIGMENU_H
