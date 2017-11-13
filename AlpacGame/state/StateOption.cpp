#include "StateOption.h"

void StateOption::goNext(StateMachine &stateMachine) {

    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    menuGUI = machine->configWindow.getMenuGUI();

    /// TGUI setup
    menuGUI->removeAllWidgets();
    //windowWidth = tgui::bindWidth(machine->configWindow.getMenuGUI());
    //windowHeight = tgui::bindHeight(machine->configWindow.getMenuGUI());


    // Creates a master button
    masterButton = theme->load("Button");

    // Fullscreen button
    fullScreenCheck = theme->load("CheckBox");
    menuGUI->add(machine->configMenu.getPictureMenu());
    menuGUI->add(machine->configMenu.mapLayouts.find(ConfigMenu::MAP_LAYOUTS::SETTINGS)->second);

    while (PollOption()) {
        drawOption();
    }
}

bool StateOption::PollOption() {
    sf::Event event;
    window->pollEvent(event);
    if (event.type == sf::Event::Closed) window->close();
    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::OPTION;
}

void StateOption::drawOption() {
    window->clear(sf::Color::Black);
    menuGUI->draw();
    window->display();
}

void StateOption::videoSettings() {

    // Clears the GUI for all widgets
    menuGUI->removeAllWidgets();
    layout->removeAllWidgets();

    // Adds a picture to the GUI as the background
    menuGUI->add(pictureMenu);

    layout->setSize(windowWidth * 2 / 3, windowHeight / 6);

    // ComboBox
    tgui::ComboBox::Ptr resolutonBox = theme->load("ComboBox");
    resolutonBox->addItem("800x600");
    resolutonBox->addItem("1280x720");
    resolutonBox->addItem("1920x1080");
    resolutonBox->setSelectedItem(tempRes);
    resolutonBox->connect("ItemSelected", &StateOption::setTempRes, this);

    // Resoulution label
    tgui::Label::Ptr resLabel = tgui::Label::create();
    resLabel->setText("Resoulution");
    resLabel->setTextSize(30);
    resLabel->setSize(100, 50);

    // TODO: fix full screen checkbox isChecked() ...
    // Fullscreen checkbox
    fullScreenCheck->setPosition(windowWidth / 6, windowHeight / 2.5);
    fullScreenCheck->setText("Fullscreen");

    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);
    // Connects a function to the  Back button
    // this function runs when the the button is pressed,
   // backButton->connect("pressed", &StateOption::menuSettings, this);

    // Apply changes button
    tgui::Button::Ptr applyButton = tgui::Button::copy(masterButton);
    applyButton->setText("Apply changes");
    applyButton->setSize(200, 50);
    applyButton->setPosition(windowWidth - 210, windowHeight - 60);
    // Connects a function to the Apply changes button
    // this function runs when the the button is pressed,
    applyButton->connect("Pressed", &StateOption::applyChanges, this);

    // Adds the widgets and layout to the GUI
    layout->add(resLabel);
    layout->add(resolutonBox);
    menuGUI->add(fullScreenCheck);
    menuGUI->add(backButton);
    menuGUI->add(applyButton);
    menuGUI->add(layout);
}

void StateOption::controlSettings() {
    menuGUI->removeAllWidgets();
    layout->removeAllWidgets();
    menuGUI->add(pictureMenu);

    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);

    // Connects a function to the  back button
    //backButton->connect("pressed", &StateOption::menuSettings, this);

    menuGUI->add(backButton);
}

void StateOption::soundSettings() {

    // TODO: add function to all the sound Sliders and sound Buttons ...
    menuGUI->removeAllWidgets();
    layout->removeAllWidgets();
    menuGUI->add(pictureMenu);

    tgui::HorizontalLayout::Ptr horiLayout = tgui::HorizontalLayout::create();

    // Scrollbar master
    tgui::Slider::Ptr masterSlider = theme->load("Slider");
    masterSlider->setValue(initialVolumeLevel);


    // Scrollbar music
    tgui::Slider::Ptr musicSlider = theme->load("Slider");
    musicSlider->setValue(initialVolumeLevel);

    // Scrollbar SFX sound
    tgui::Slider::Ptr sfxSlider= theme->load("Slider");
    sfxSlider->setValue(initialVolumeLevel);

    // Slider master label
    tgui::Label::Ptr masterLabel = tgui::Label::create();
    masterLabel->setText("Master volume");


    // Scrollbar music label
    tgui::Label::Ptr musicLabel = tgui::Label::create();
    musicLabel->setText("Music volume");


    // Slider SFX label
    tgui::Label::Ptr sfxLabel = tgui::Label::create();
    sfxLabel->setText("Sound effects");

    // Mute sound checkbox
    tgui::CheckBox::Ptr muteSound = theme->load("CheckBox");
    muteSound->setText("Mute Sound");

    // Apply changes button
    tgui::Button::Ptr applyButton = tgui::Button::copy(masterButton);
    applyButton->setText("Apply changes");
    applyButton->setSize(200, 50);
    applyButton->setPosition(windowWidth - 210, windowHeight - 60);
    applyButton->connect("Pressed", [this, musicSlider] { std::cout << musicSlider->getValue() << std::endl; });

    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);
    // Connects a function to the  Back button
    // this function runs when the the button is pressed,
    //backButton->connect("pressed", &StateOption::menuSettings, this);

    // Adds all the layouts and widgets to the GUI
    horiLayout->add(muteSound);
    horiLayout->addSpace(2.75);
    layout->add(horiLayout);
    layout->addSpace();
    layout->addSpace();
    layout->add(masterLabel);
    layout->add(masterSlider);
    layout->addSpace();
    layout->add(musicLabel);
    layout->add(musicSlider);
    layout->addSpace();
    layout->add(sfxLabel);
    layout->add(sfxSlider);
    menuGUI->add(applyButton);
    menuGUI->add(layout);
    menuGUI->add(backButton);

}

void StateOption::setResoulution(std::string res) {
    if (res == "800x600") {
        StateOption::lowRes();
        tempXres = 800;
        tempYres = 600;
    } else if (res == "1280x720") {
        StateOption::medRes();
        tempXres = 1280;
        tempYres = 720;
    } else if (res == "1920x1080") {
        StateOption::highRes();
        tempXres = 1920;
        tempYres = 1080;
    }

}

void StateOption::lowRes() {
    machine->configWindow.setScreenWidth(800);
    machine->configWindow.setScreenHeight(600);
    if (fullScreenCheck->isChecked()) {
        machine->configWindow.setFullscreeWindowResolution(800, 600);
    } else {
        machine->configWindow.setWindowResolution(800, 600);
    }
}

void StateOption::medRes() {
    machine->configWindow.setScreenWidth(1280);
    machine->configWindow.setScreenHeight(720);
    if (fullScreenCheck->isChecked()) {
        machine->configWindow.setFullscreeWindowResolution(1280, 720);
    } else {
        machine->configWindow.setWindowResolution(1280, 720);
    }
}

void StateOption::highRes() {
    machine->configWindow.setScreenWidth(1920);
    machine->configWindow.setScreenHeight(1080);
    if (fullScreenCheck->isChecked()) {
        machine->configWindow.setFullscreeWindowResolution(1920, 1080);
    } else {
        machine->configWindow.setWindowResolution(1920, 1080);
    }
}

void StateOption::applyChanges() {
    std::cout << tempRes << std::endl;
    if (fullScreenCheck->isChecked() == 1) {
        // set fullscreen
        // TODO: implement functional setFullscreen
        machine->configWindow.setScreenWidth(tempXres);
        machine->configWindow.setScreenHeight(tempYres);
        StateOption::setResoulution(tempRes);
    } else {
        // not fullscreen
        machine->configWindow.setScreenWidth(tempXres);
        machine->configWindow.setScreenHeight(tempYres);
        StateOption::setResoulution(tempRes);
    }
}

void StateOption::setTempRes(const std::string &tempRes) {
    StateOption::tempRes = tempRes;
}