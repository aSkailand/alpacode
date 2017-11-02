#include "ConfigWindow.h"

void ConfigWindow::run() {
    sf::VideoMode windowResolution = sf::VideoMode(screenWidth, screenHeight,
                                                   32);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);

    menuGUI.removeAllWidgets();
    menuGUI.setWindow(window);
}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

tgui::Gui &ConfigWindow::getMenuGUI() {
    return menuGUI;
}

void ConfigWindow::setWindowResolution(int unsigned screenWidth, int unsigned screenHeight) {
    window.create(sf::VideoMode(screenWidth, screenHeight, 32), "AlpaGame");
}

void ConfigWindow::setFullscreeWindowResolution(int unsigned screenWidth, int unsigned screenHeight) {
    window.create(sf::VideoMode(screenWidth, screenHeight, 32), "AlpaGame", sf::Style::Fullscreen);
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

