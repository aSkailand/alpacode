#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>
#include "ConfigMenu.h"

class ConfigWindow {
public:

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
    void setWindowResolution(ConfigMenu::resolution resolution, uint32_t style);

private:
    int screenWidth = 800;
    int screenHeight = 600;
    sf::RenderWindow window;
    tgui::Gui *menuGUI;
};


#endif //ALPACGAME_CONFIG_H
