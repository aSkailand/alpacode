#include "ConfigWindow.h"

void ConfigWindow::run(){
    sf::VideoMode windowResolution = sf::VideoMode(800, 600);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);

    menuGUI.setWindow(window);
}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

tgui::Gui &ConfigWindow::getMenuGUI(){
    return menuGUI;
}

void ConfigWindow::setWindowResolution(int unsigned screenWidth, int unsigned screenHeight) {
    sf::VideoMode windowResolution = sf::VideoMode(screenWidth, screenHeight);
    window.create(windowResolution, "Alpagame");
}
