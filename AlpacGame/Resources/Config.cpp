
#include "Config.h"

void Config::run(){

    windowResolution = sf::VideoMode(windowWidth,windowHeight);  // todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);

}

sf::RenderWindow &Config::getWindow() {
    return window;
}