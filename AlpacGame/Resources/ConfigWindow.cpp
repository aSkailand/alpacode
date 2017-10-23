
#include "ConfigWindow.h"

void ConfigWindow::run(){
    windowResolution = sf::VideoMode(windowWidth,windowHeight);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);
}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

void ConfigWindow::setWindowResolution(int unsigned screenWidth, int unsigned screenHeight) {
    windowResolution = sf::VideoMode(screenWidth, screenHeight);
    window.create(windowResolution, "Alpagame");
}