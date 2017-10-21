
#include "Config.h"

void Config::run(){
    windowResolution = sf::VideoMode(screenWidth, screenHeight);  // todo: place this line another place
    Config::window.create(windowResolution, "AlpaGame");
}

sf::RenderWindow &Config::getWindow() {
    return window;
}
