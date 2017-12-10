#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>

/**
 * Handles the configurations of the window. Used mostly as a resource manager when
 * it comes to the common window.
 */
class ConfigWindow {
public:

    /// Enums
    enum class Resolution {
        RES800x600 = 0,
        RES1280x720 = 1,
        RES1920x1080 = 2,
        RES2160x1440 = 3
    };

    /**
     * Current window resolution.
     */
    Resolution currentResolution = ConfigWindow::Resolution::RES800x600;

    /// Config Functions
    /**
     * Initiate config and creates all resolutions and the common window itself.
     */
    void run();

    /**
     * Recreate the common window to the chosen resolution and style.
     * @param resolution the resolution of the window.
     * @param style the style the window will use (either default or fullscreen).
     */
    void setWindowResolution(ConfigWindow::Resolution resolution, uint32_t style);

    /**
     * Convert the mappings of resolution and strings to resolution and sf::VideoModes
     * to a the map mapResolutionString.
     */
    void fillMapResolutionVideoMode();

    /// Getters
    tgui::Gui *getMenuGUI();
    sf::RenderWindow &getWindow();

    /// Public Map
    /**
     * @note Exists alongside with the map mapResolutionVideoMode
     * because configMenu's comboBox handles strings.
     */
    std::map<Resolution, std::string> mapResolutionString;

private:

    /// Pointers
    sf::RenderWindow window;
    tgui::Gui *menuGUI = nullptr;

    /// Private Map
    /**
     * @note Exists alongside with the map mapResolutionString
     * because SFML changes resolution through sf::VideoMode.
     */
    std::map<Resolution, sf::VideoMode> mapResolutionVideoMode;
};


#endif //ALPACGAME_CONFIG_H
