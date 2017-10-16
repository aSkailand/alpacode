
#include "ConfigWindow.h"

void ConfigWindow::run(){

    windowResolution = sf::VideoMode(windowWidth,windowHeight);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);

}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}