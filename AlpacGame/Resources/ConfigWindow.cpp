#include "ConfigWindow.h"

void ConfigWindow::run() {

    // Map resolutions to string
    mapResolutionString[Resolution::RES800x600] = "800x600";
    mapResolutionString[Resolution::RES1280x720] = "1280x720";
    mapResolutionString[Resolution::RES1920x1080] = "1920x1080";

    // Map resolutions to sf::VideoMode
    fillMapResolutionVideoMode();

    // Set current resolution
    setWindowResolution(currentResolution, sf::Style::Default);

    // Setup TGUI
    menuGUI = new tgui::Gui();
    menuGUI->removeAllWidgets();
    menuGUI->setWindow(window);
    menuGUI->setFont("Resources/PressStart2P.ttf");

}

sf::RenderWindow &ConfigWindow::getWindow() {
    return window;
}

tgui::Gui *ConfigWindow::getMenuGUI() {
    return menuGUI;
}

void ConfigWindow::setWindowResolution(ConfigWindow::Resolution resolution, uint32_t style) {

    sf::VideoMode currentVideoMode = mapResolutionVideoMode[resolution];
    window.create(currentVideoMode, "AlpaGame", style);
    window.setFramerateLimit(144);

}

void ConfigWindow::fillMapResolutionVideoMode() {

    for (auto &iter : mapResolutionString) {

        std::string s = iter.second;
        std::string delimiter = "x";

        unsigned int width = static_cast<unsigned int>(std::stoi(s.substr(0, s.find(delimiter))));
        unsigned int height = static_cast<unsigned int>(std::stoi(s.substr(s.find(delimiter)+1, s.find(delimiter))));

        mapResolutionVideoMode[iter.first] = sf::VideoMode(width, height);
    }

}

