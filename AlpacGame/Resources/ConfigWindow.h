#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>

class ConfigWindow {
public:

    enum class Resolution {
        RES800x600 =    0,
        RES1280x720 =   1,
        RES1920x1080 =  2
    };

    /// Pointers
    tgui::Gui *getMenuGUI();

    sf::RenderWindow &getWindow();

    /**
     * Runs and creates the window
     */
    void run();

    /**
     * Sets the window resolution to the desired width and height
     * @param screenWidth
     * @param screenHeight
     */
    void setWindowResolution(ConfigWindow::Resolution resolution, uint32_t style);

    std::map<Resolution, std::string> mapResolution;

    Resolution currentResolution = ConfigWindow::Resolution::RES800x600;

private:
    sf::RenderWindow window;
    tgui::Gui *menuGUI;
};


#endif //ALPACGAME_CONFIG_H
