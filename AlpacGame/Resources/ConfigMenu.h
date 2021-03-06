#ifndef ALPACGAME_CONFIGMENU_H
#define ALPACGAME_CONFIGMENU_H

#include <map>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "ConfigWindow.h"

/**
 * Using a forward-declaration here to avoid a recursion of includes between ConfigMenu and
 * StateMachine.
 */
class StateMachine;

/**
 * This class creates and holds all of the layouts and widgets of the menu GUI.
 */
class ConfigMenu {
public:

    /// Enums
    enum class layouts {
        MAINMENU,
        HIGHSCORE,
        SETTINGS,
        VIDEO,
        SOUND,
        CONTROLS,
        PAUSE,
        DEFEAT
    };

    enum class buttonID {
        NEWGAME,
        RESUME,
        HIGHSCORE,
        SETTINGS,
        QUIT,
        VIDEO,
        SOUND,
        CONTROLS,
        BACK_HIGHSCORE,
        BACK_TO_MAIN,
        APPLY_SETTINGS,
        MASTER_SLIDER,
        MUSIC_SLIDER,
        EFFECT_SLIDER
    };

    /// Public Functions
    /**
     * Defines the GUI of menu and create all needed buttons, then bind it to the common window.
     * @param stateMachine the reference to the common stateMachine.
     */
    void run(StateMachine &stateMachine);

    /// Public Getter
    /**
     * Getter for the background picture, this image is only loaded once to save resources.
     * @return returns a tgui::Picture
     */
    tgui::Picture::Ptr &getPictureMenu();

    tgui::Picture::Ptr &getPictureTittle();

    /// Mappings of the different layouts and widgets
    std::map<buttonID, tgui::Button::Ptr> mapButtons;
    std::map<layouts, tgui::VerticalLayout::Ptr> mapLayouts;
    std::map<buttonID, tgui::Slider::Ptr> mapSliders;
    std::map<std::string, int> mapHighscore;

    /**
     * The getter for the score label
     * @return
     */
    tgui::Label::Ptr getScoreLabel();

    /**
     * Sets the score label text
     * @param score, takes in a String that is the new score
     */
    void setScore(std::string score);

    /// KEYMAPPING
    tgui::Button::Ptr shootKey;
    tgui::Button::Ptr moveLeftKey;
    tgui::Button::Ptr moveRightKey;
    tgui::Button::Ptr jumpKey;
    tgui::Button::Ptr grabThrowKey;
    tgui::Button::Ptr zoomKey;

    /// Changes made
    bool changesMadeVideo = false;

    bool changesMadeSound = true;

    bool changesMadeControls = false;

    bool insideVideoSetting = true;

    bool insideControlSetting = false;

    bool insideSoundSettings = false;

    /**
     * Checks if all the buttons got their default binds
     * and if they do, disables the default button
     */
    void defaultKeysCheck();

    tgui::Label::Ptr allAlpacasDead;
    tgui::Label::Ptr farmerDead;

    tgui::EditBox::Ptr nameEditBox;

private:

    /// Pointers
    StateMachine *machine = nullptr;

    /// TGUI objects
    tgui::VerticalLayout::Ptr tempVerticalLayout;
    tgui::Picture::Ptr pictureMenu;
    tgui::Picture::Ptr pictureTittleMenu;
    tgui::Button::Ptr masterButton;
    tgui::Theme::Ptr theme;
    tgui::Layout windowWidth;
    tgui::Layout windowHeight;
    tgui::TextBox::Ptr textBoxHighscore;
    tgui::Label::Ptr labelScoreHUD;


    /// CheckBox
    bool checkBoxMuteChecked = false;

    bool checkBoxFullScreenChecked = false;

    /// Current fullscreen
    bool isFullScreen = false;

    /// Current Resolution
    std::string currentResolution = "800x600";

    /// Layout Functions
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
     * Creates the control settings layout
     */
    void controlSettingsLayout();

    /**
     * Creates the sound settings layout
     */
    void soundSettingsLayout();

    /**
     * Creates the pause menu layout
     */
    void pauseMenuLayout();

    /**
     * Creates the highscore layout
     */
    void highscoreLayout();

    /**
     * Takes a text file and writes the content into a text box
     * @param highScoreFile
     */
    void loadHighscore(std::string highScoreFile);

    /**
     * Creates the defeat screen layout
     */
    void defeatScreenLayout();

    /**
     * Creates the defeat labels
     */
    void defeatTitleLabel();

    /**
     * Creates the score label
     */
    void scoreLabel();

    /// Button Functions
    /**
     * Creates a button with the given parameters, then store it in a map for later accessing.
     * @param buttonID the button identifier, which is an enum.
     * @param buttonName the button name, which will be visually presented during run-time.
     * @param typeActivation how the button is activated, E.g. pressed, released.
     * @param func binds a function to the button, this function runs when the button is activated
     */
    void createButton(
            buttonID buttonID,
            const std::string &buttonName,
            const std::string &typeActivation,
            const std::function<void()> &func
    );

    /**
   * Creates a slider
   * @param sliderType the slider identifier, used as the key in the mappings.
   */
    void createSlider(buttonID sliderType);

    /// Private Setters and Getters
    /**
 * Getter for the current resolution.
 * @return returns the current resolution
 */
    ConfigWindow::Resolution getCurrentResolution() const;

    /**
     * Sets the current resolution
     * @param currentResolution
     */
    void setCurrentResolution(ConfigWindow::Resolution currentResolution);

    void applyChanges();

    /**
     * Creating a function to reset all button text exept the chosen one.
     */
    void controlButtonTextReset();

    void optionInsideSwitcher();
};


#endif //ALPACGAME_CONFIGMENU_H
