#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>

class ConfigWindow {
public:

    /// Pointers
    tgui::Gui &getMenuGUI();

    sf::RenderWindow &getWindow();

    /**
     * Getter for screenWidth
     * @return screenWidth
     */
    int getScreenWidth() const;

    /**
     * Getter for screenHeight
     * @return screenHeight
     */
    int getScreenHeight() const;

    /**
     * Setter for screenWidth
     * @param screenWidth
     */
    void setScreenWidth(int screenWidth);

    /**
     * Setter for screenHeight
     * @param screenHeight
     */
    void setScreenHeight(int screenHeight);

    /**
     * Runs and creates the window
     */
    void run();

    /**
     * Sets the window resolution to the desired width and height
     * @param screenWidth
     * @param screenHeight
     */
    void setWindowResolution(int unsigned screenWidth, int unsigned screenHeight);

    /**
     * Sets the window resolution to the desired width and height with a fullscreen style
     * @param screenWidth
     * @param screenHeight
     */
    void setFullscreeWindowResolution(int unsigned screenWidth, int unsigned screenHeight);

private:

    int screenWidth = 800;
    int screenHeight = 600;
    sf::RenderWindow window;
    tgui::Gui menuGUI;
};


#endif //ALPACGAME_CONFIG_H
