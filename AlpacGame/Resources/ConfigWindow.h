#ifndef ALPACGAME_CONFIG_H
#define ALPACGAME_CONFIG_H

#include <TGUI/TGUI.hpp>

class ConfigWindow {
public:

    int screenWidth = 800;
    int screenHeight = 600;

    int getScreenWidth() const;

    int getScreenHeight() const;

    void setScreenWidth(int screenWidth);

    void setScreenHeight(int screenHeight);


    void run();

    tgui::Gui &getMenuGUI();
    sf::RenderWindow &getWindow();
    void setWindowResolution(int unsigned screenWidth, int unsigned screenHeight);



private:
    sf::RenderWindow window;
    tgui::Gui menuGUI;

};


#endif //ALPACGAME_CONFIG_H
