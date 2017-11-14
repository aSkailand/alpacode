#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>

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
    void setWindowResolution(int resolution);

    /**
     * Sets the window resolution to the desired width and height with a fullscreen style
     * @param screenWidth
     * @param screenHeight
     */
    void setFullScreenWindowResolution(int resolution);

private:
    int screenWidth = 800;
    int screenHeight = 600;
    sf::RenderWindow window;
    tgui::Gui *menuGUI;
};


#endif //ALPACGAME_CONFIG_H
