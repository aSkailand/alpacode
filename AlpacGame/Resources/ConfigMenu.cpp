
#include "ConfigMenu.h"
#include "../state/StateMachine.h"

void ConfigMenu::run(StateMachine &stateMachine) {

    this->machine = &stateMachine;

    theme = tgui::Theme::create("Resources/BabyBlue.txt");
    pictureMenu = tgui::Picture::create("Resources/aluminium.jpg");
    masterButton = theme->load("Button");

    createButton(buttonID::PLAYGAME, "Play game!", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER); });

    createButton(buttonID::SETTINGS, "Settings", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::OPTION); });

    createButton(buttonID::QUIT, "Quit", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::EXIT); });

    createButton(buttonID::BACK_TO_MAIN, "Back", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::MENU); });

    createButton(buttonID::VIDEO, "Video", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->configWindow.getMenuGUI()->add(getPictureMenu());
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::SETTINGS]);
                     machine->configWindow.getMenuGUI()->add(mapLayouts[ConfigMenu::layouts::VIDEO]);
                 });

    createButton(buttonID::CONTROLS, "Controls", "pressed",
                 [&] {});

    createButton(buttonID::SOUND, "Sound", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->configWindow.getMenuGUI()->add(getPictureMenu());
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::SETTINGS]);
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::SOUND]);

                 });


    createButton(buttonID::APPLY_VIDEO_SETTINGS, "Apply changes", "pressed",
                 [&] {
                     if (checkBoxFullScreenChecked) {
                         machine->configWindow.setWindowResolution(getCurrentResolution(), sf::Style::Fullscreen);
                     } else {
                         machine->configWindow.setWindowResolution(getCurrentResolution(), sf::Style::Default);
                     }
                 });

    createButton(buttonID::APPLY_SOUND_SETTINGS, "Apply changes", "pressed",
                 [&] {
                     machine->configSound.setMasterVolume(mapSliders[buttonID::MASTER_SLIDER]->getValue());
                     machine->configSound.setMusicVolume(mapSliders[buttonID::MUSIC_SLIDER]->getValue());
                     machine->configSound.setSoundEffects(mapSliders[buttonID::EFFECT_SLIDER]->getValue());
                 });

    createSlider(buttonID::MASTER_SLIDER);
    createSlider(buttonID::MUSIC_SLIDER);
    createSlider(buttonID::EFFECT_SLIDER);

    mainMenuLayout(machine->configWindow.getMenuGUI());
}

tgui::Picture::Ptr &ConfigMenu::getPictureMenu() {
    return pictureMenu;
}

void ConfigMenu::createButton(buttonID buttonID,
                              const std::string &buttonName,
                              const std::string &typeActivation,
                              const std::function<void()> &func) {
    tgui::Button::Ptr tempButton = tgui::Button::copy(masterButton);
    tempButton->setText(buttonName);
    tempButton->connect(typeActivation, func);
    mapButtons.emplace(buttonID, tempButton);
}


void ConfigMenu::mainMenuLayout(tgui::Gui *Width) {

    // Creates the different layouts
    // Main menu layout
    tempVerticalLayout = tgui::VerticalLayout::create();
    pictureMenu->setSize(tgui::bindMax(800, windowWidth), tgui::bindMax(600, windowHeight));
    tempVerticalLayout->removeAllWidgets();
    windowWidth = tgui::bindWidth(*Width);
    windowHeight = tgui::bindHeight(*Width);
    tempVerticalLayout->setSize(windowWidth * 2 / 3, windowHeight / 2);
    tempVerticalLayout->setPosition(windowWidth / 6, windowHeight / 6);

    tempVerticalLayout->add(mapButtons[buttonID::PLAYGAME]);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(mapButtons[buttonID::SETTINGS]);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(mapButtons[buttonID::QUIT]);

    // Adds the main menu layout to the map
    mapLayouts.emplace(layouts::MAINMENU, tempVerticalLayout);

    // Settings layout
    tgui::VerticalLayout::Ptr optionsVerticalLayout = tgui::VerticalLayout::create();
    optionsVerticalLayout->setSize(windowWidth / 4, windowHeight);
    optionsVerticalLayout->setPosition(windowWidth - (windowWidth - 10), 0);
    optionsVerticalLayout->removeAllWidgets();
    optionsVerticalLayout->addSpace(0.5f);
    optionsVerticalLayout->add(mapButtons[buttonID::VIDEO]);
    optionsVerticalLayout->addSpace(2);
    optionsVerticalLayout->add(mapButtons[buttonID::CONTROLS]);
    optionsVerticalLayout->addSpace(2);
    optionsVerticalLayout->add(mapButtons[buttonID::SOUND]);
    optionsVerticalLayout->addSpace(6);
    optionsVerticalLayout->add(mapButtons[buttonID::BACK_TO_MAIN]);
    optionsVerticalLayout->addSpace(0.5f);

    // Adds the settings layout to the map
    mapLayouts.emplace(layouts::SETTINGS, optionsVerticalLayout);

    videoSettingsLayout();
    soundSettingsLayout();
}

void ConfigMenu::videoSettingsLayout() {

    tgui::VerticalLayout::Ptr videoSettingsLayout = tgui::VerticalLayout::create();
    tgui::HorizontalLayout::Ptr hori = tgui::HorizontalLayout::create();
    videoSettingsLayout->setSize(windowWidth / 4, windowHeight / 2);
    videoSettingsLayout->setPosition(windowWidth / 3, windowHeight / 6);

    // ComboBox
    tgui::ComboBox::Ptr resolutionBox = theme->load("ComboBox");

    // Add all possible options inside ComboBox
    for (auto &iter : machine->configWindow.mapResolutionString) {
        resolutionBox->addItem(iter.second);
    }

    // Set initial selection
    resolutionBox->setSelectedItem(machine->configWindow.mapResolutionString[machine->configWindow.currentResolution]);

    // Setup selection mechanism
    resolutionBox->connect("ItemSelected",
                           [&](std::string itemSelected) {
                               for (auto &iter : machine->configWindow.mapResolutionString) {
                                   if(itemSelected == iter.second)
                                       setCurrentResolution(iter.first);
                               }
                           });

    // Resolution label
    tgui::Label::Ptr resLabel = tgui::Label::create();
    resLabel->setText("Resolution");
    resLabel->setTextSize(30);
    resLabel->setSize(100, 50);


    // Fullscreen checkbox
    tgui::CheckBox::Ptr fullScreenCheck = theme->load("CheckBox");
    fullScreenCheck->setPosition(windowWidth / 6, windowHeight / 2.5);
    fullScreenCheck->setText("Fullscreen");
    fullScreenCheck->setTextSize(24);
    fullScreenCheck->connect("clicked", [&] { checkBoxFullScreenChecked = !checkBoxFullScreenChecked; });

    // Adds the widgets and layout to the GUI
    videoSettingsLayout->add(resLabel);
    videoSettingsLayout->addSpace();
    videoSettingsLayout->add(resolutionBox);
    videoSettingsLayout->addSpace();
    hori->add(fullScreenCheck);
    hori->addSpace(5);
    videoSettingsLayout->add(hori);
    videoSettingsLayout->addSpace(5);
    videoSettingsLayout->add(mapButtons[buttonID::APPLY_VIDEO_SETTINGS]);

    mapLayouts.emplace(layouts::VIDEO, videoSettingsLayout);
}

void ConfigMenu::soundSettingsLayout() {

    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    tgui::HorizontalLayout::Ptr horiLayout = tgui::HorizontalLayout::create();
    layout->setSize(windowWidth / 4, windowHeight / 2);
    layout->setPosition(windowWidth / 3, windowHeight / 6);

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
    muteSound->connect("clicked", [&] { checkBoxMuteChecked = !checkBoxMuteChecked; });

    // Back button
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->setSize(100, 50);
    backButton->setPosition(10, windowHeight - 60);

    // Adds all the layouts and widgets to the GUI
    horiLayout->add(muteSound);
    horiLayout->addSpace(5);
    layout->add(horiLayout);
    layout->addSpace();
    layout->addSpace();
    layout->add(masterLabel);
    layout->add(mapSliders[buttonID::MASTER_SLIDER]);
    layout->addSpace();
    layout->add(musicLabel);
    layout->add(mapSliders[buttonID::MUSIC_SLIDER]);
    layout->addSpace();
    layout->add(sfxLabel);
    layout->add(mapSliders[buttonID::EFFECT_SLIDER]);
    layout->addSpace();
    layout->add(mapButtons[buttonID::APPLY_SOUND_SETTINGS]);

    mapLayouts.emplace(layouts::SOUND, layout);
}

ConfigWindow::Resolution ConfigMenu::getCurrentResolution() const {
    return machine->configWindow.currentResolution;
}

void ConfigMenu::setCurrentResolution(ConfigWindow::Resolution currentResolution) {
    machine->configWindow.currentResolution = currentResolution;
}

void ConfigMenu::createSlider(buttonID sliderType) {
    // Temporary slider
    tgui::Slider::Ptr tempSlider = theme->load("Slider");
    mapSliders.emplace(sliderType, tempSlider);
}
