
#include "Config.h"

void Config::run(){
    windowResolution = sf::VideoMode(800,600);  // todo: place this line another place
    Config::window.create(windowResolution, "AlpaGame");
}

sf::RenderWindow &Config::getWindow() {
    return window;
}
