
#include "Config.h"

void Config::run(){
    windowResolution = sf::VideoMode(800,600);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);
}

sf::RenderWindow &Config::getWindow() {
    return window;
}
