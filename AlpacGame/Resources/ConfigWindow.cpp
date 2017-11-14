#include "ConfigWindow.h"

void ConfigWindow::run() {

    // Fill map with resolutions
    mapResolution[Resolution::RES800x600] = "800x600";
    mapResolution[Resolution::RES1280x720] = "1280x720";
    mapResolution[Resolution::RES1920x1080] = "1920x1080";

    // Set default resolution
    setWindowResolution(currentResolution, sf::Style::Default);
    window.setFramerateLimit(144);

    // Setup TGUI
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

void ConfigWindow::setWindowResolution(ConfigWindow::Resolution resolution, uint32_t style) {

    std::string s = mapResolution[resolution];
    std::string delimiter = "x";
    unsigned int width = static_cast<unsigned int>(std::stoi(s.substr(0, s.find(delimiter))));
    unsigned int height = static_cast<unsigned int>(std::stoi(s.substr(s.find(delimiter)+1, s.find(delimiter))));

    window.create(sf::VideoMode(width, height, 32), "AlpaGame", style);

}

