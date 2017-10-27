#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>

class ConfigWindow {
public:
    void run();

    tgui::Gui &getGUI();
    sf::RenderWindow &getWindow();
    void setWindowResolution(int unsigned screenWidth, int unsigned screenHeight);

private:
    sf::RenderWindow window;
    tgui::Gui gui{window};
};


#endif //ALPACGAME_CONFIG_H
