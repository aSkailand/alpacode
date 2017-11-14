#include "ConfigWindow.h"

void ConfigWindow::run() {
    sf::VideoMode windowResolution = sf::VideoMode(screenWidth, screenHeight, 32);// todo: place this line another place
    window.create(windowResolution, "AlpaGame");
    window.setFramerateLimit(144);
    menuGUI = new tgui::Gui();
    menuGUI->removeAllWidgets();
    menuGUI->setWindow(window);
}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

tgui::Gui *ConfigWindow::getMenuGUI() {
    return menuGUI;
}

void ConfigWindow::setWindowResolution(int resolution) {
    if (resolution == 0) {
        window.create(sf::VideoMode(800, 600, 32), "AlpaGame", sf::Style::Default);
    } else if (resolution == 1) {
        window.create(sf::VideoMode(1280, 720, 32), "AlpaGame", sf::Style::Default);
    } else if (resolution == 2) {
        window.create(sf::VideoMode(1920, 1080, 32), "AlpaGame", sf::Style::Default);
    }
}

void ConfigWindow::setFullScreenWindowResolution(int resolution) {
    if (resolution == 0) {
        window.create(sf::VideoMode(800, 600, 32), "AlpaGame", sf::Style::Fullscreen);
    } else if (resolution == 1) {
        window.create(sf::VideoMode(1280, 720, 32), "AlpaGame", sf::Style::Fullscreen);
    } else if (resolution == 2) {
        window.create(sf::VideoMode(1920, 1080, 32), "AlpaGame", sf::Style::Fullscreen);
    }
}
