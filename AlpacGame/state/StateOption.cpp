#include "StateOption.h"


void StateOption::goNext(StateMachine &stateMachine) {


    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    menuGUI = &machine->configWindow.getMenuGUI();
    menuGUI->removeAllWidgets();
    windowWidth = tgui::bindWidth(machine->configWindow.getMenuGUI());
    windowHeight = tgui::bindHeight(machine->configWindow.getMenuGUI());

    initialVolumeLevel = 10;


    tempXres = 800;
    tempYres = 600;
    tempRes = "800x600";


    // Picture
    picture = tgui::Picture::create("aluminium.jpg");
    picture->setSize(tgui::bindMax(machine->configWindow.getScreenWidth(), windowWidth),
                     tgui::bindMax(machine->configWindow.getScreenHeight(), windowHeight));


    // Creates a layout
    layout = tgui::VerticalLayout::create();
    layout->setSize(windowWidth * 2 / 3, windowHeight / 2);
    layout->setPosition(windowWidth / 6, windowHeight / 6);

    // Creates a master button
    masterButton = theme->load("Button");

    // Fullscreen button
    fullScreenCheck = theme->load("CheckBox");

    menuSettings();

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

void StateOption::menuSettings() {
    picture->setSize(tgui::bindMax(machine->configWindow.getScreenWidth(), windowWidth),
                     tgui::bindMax(machine->configWindow.getScreenHeight(), windowHeight));
    layout->setSize(windowWidth * 2 / 3, windowHeight / 2);
    layout->setPosition(windowWidth / 6, windowHeight / 6);

    menuGUI->removeAllWidgets();
    layout->removeAllWidgets();

    menuGUI->add(picture);

    // Video button
    tgui::Button::Ptr videoSettings = tgui::Button::copy(masterButton);
    videoSettings->setText("Video settings");
    videoSettings->connect("pressed", StateOption::videoSettings, this);

    // Control button
    tgui::Button::Ptr controlSettings = tgui::Button::copy(masterButton);
    controlSettings->setText("Controls");
    controlSettings->connect("pressed", StateOption::controlSettings, this);

    // Sound button
    tgui::Button::Ptr soundSettings = tgui::Button::copy(masterButton);
    soundSettings->setText("Sound settings");
    soundSettings->connect("pressed", this->soundSettings, this);


    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);

    // Connects a function to the  back button
    backButton->connect("pressed", [this] { machine->setCurrentState(StateMachine::stateID::MENU); });


    // Adds all the widgets to the layout and GUI
    layout->add(videoSettings);
    layout->addSpace();
    layout->add(controlSettings);
    layout->addSpace();
    layout->add(soundSettings);
    layout->addSpace();
    menuGUI->add(layout);
    menuGUI->add(backButton);
}

void StateOption::videoSettings() {

    // Setup
    menuGUI->removeAllWidgets();
    layout->removeAllWidgets();
    picture->setSize(tgui::bindMax(800, windowWidth), tgui::bindMax(600, windowHeight));
    menuGUI->add(picture);
    layout->setSize(windowWidth * 2 / 3, windowHeight / 6);

    // ComboBox
    tgui::ComboBox::Ptr resolutonBox = theme->load("ComboBox");
    resolutonBox->addItem("800x600");
    resolutonBox->addItem("1280x720");
    resolutonBox->addItem("1920x1080");
    resolutonBox->setSelectedItem(tempRes);
    resolutonBox->connect("ItemSelected", StateOption::setTempRes, this);

    // Resoulution label
    tgui::Label::Ptr resLabel = tgui::Label::create();
    resLabel->setText("Resoulution");
    resLabel->setTextSize(30);
    resLabel->setSize(100, 50);


    // TODO: fix ful screen checkbox isChecked() ...
    // TODO: fix fullscreen feature ...
    // Fullscreen checkbox
    fullScreenCheck->setPosition(windowWidth / 6, windowHeight / 2.5);
    fullScreenCheck->setText("Fullscreen");



    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);
    backButton->connect("pressed", StateOption::menuSettings, this);

    // Apply changes button
    tgui::Button::Ptr applyButton = tgui::Button::copy(masterButton);
    applyButton->setText("Apply changes");
    applyButton->setSize(200, 50);
    applyButton->setPosition(windowWidth - 210, windowHeight - 60);
    applyButton->connect("Pressed", StateOption::applyChanges, this);

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
    menuGUI->add(picture);

    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);

    // Connects a function to the  back button
    backButton->connect("pressed", StateOption::menuSettings, this);

    menuGUI->add(backButton);
}

void StateOption::soundSettings() {
    menuGUI->removeAllWidgets();
    layout->removeAllWidgets();
    menuGUI->add(picture);

    tgui::HorizontalLayout::Ptr horiLayout = tgui::HorizontalLayout::create();


    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);



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


    backButton->connect("pressed", this->menuSettings, this);

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