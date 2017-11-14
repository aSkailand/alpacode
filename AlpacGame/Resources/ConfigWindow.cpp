#include "ConfigWindow.h"

void ConfigWindow::run() {
    sf::VideoMode windowResolution = sf::VideoMode(screenWidth, screenHeight, 32);// todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);
    menuGUI = new tgui::Gui();
    menuGUI->removeAllWidgets();
    menuGUI->setWindow(window);
}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

tgui::Gui *ConfigWindow::getMenuGUI() {
    return menuGUI;
}

void ConfigWindow::setWindowResolution(ConfigMenu::resolution resolution, uint32_t style) {

    switch (resolution) {
        case ConfigMenu::resolution::RES800x600: {
            window.create(sf::VideoMode(800, 600, 32), "AlpaGame", style);
            break;
        }
        case ConfigMenu::resolution::RES1280x720: {
            window.create(sf::VideoMode(1280, 720, 32), "AlpaGame", style);
            break;
        }
        case ConfigMenu::resolution::RES1920x1080: {
            window.create(sf::VideoMode(1920, 1080, 32), "AlpaGame", style);
            break;
        }
    }

}

