
#include "Config.h"

void Config::run(){
    windowResolution = sf::VideoMode(800, 600);  // todo: place this line another place
    Config::window.create(windowResolution, "AlpaGame");
}

sf::RenderWindow &Config::getWindow() {
    return window;
}

void Config::setWindowResolution(int unsigned screenWidth, int unsigned screenHeight) {
    windowResolution = sf::VideoMode(screenWidth, screenHeight);
    Config::window.create(windowResolution, "Alpagame");
}