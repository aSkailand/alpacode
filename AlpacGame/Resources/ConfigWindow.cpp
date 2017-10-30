#include "ConfigWindow.h"

void ConfigWindow::run(){
    sf::VideoMode windowResolution = sf::VideoMode(screenWidth, screenHeight);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);

    menuGUI.removeAllWidgets();
    menuGUI.setWindow(window);
}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

tgui::Gui &ConfigWindow::getMenuGUI(){
    return menuGUI;
}

void ConfigWindow::setWindowResolution(int unsigned screenWidth, int unsigned screenHeight) {
    window.setSize(sf::Vector2u(screenWidth,screenHeight));
}



void ConfigWindow::setScreenWidth(int screenWidth) {
    ConfigWindow::screenWidth = screenWidth;
}



void ConfigWindow::setScreenHeight(int screenHeight) {
    ConfigWindow::screenHeight = screenHeight;
}

int ConfigWindow::getScreenWidth() const {
    return screenWidth;
}

int ConfigWindow::getScreenHeight() const {
    return screenHeight;
}
