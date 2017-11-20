
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

    createButton(buttonID::APPLY_SETTINGS, "Apply changes", "pressed", [&] {
        ConfigMenu::applyChanges();
    });

    createSlider(buttonID::MASTER_SLIDER);
    createSlider(buttonID::MUSIC_SLIDER);
    createSlider(buttonID::EFFECT_SLIDER);
    defeatScreenLayout();

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
    optionsVerticalLayout->setPosition(windowWidth - (windowWidth), 0);
    optionsVerticalLayout->removeAllWidgets();
    optionsVerticalLayout->addSpace(0.5f);
    optionsVerticalLayout->add(mapButtons[buttonID::VIDEO]);
    optionsVerticalLayout->addSpace(1);
    optionsVerticalLayout->add(mapButtons[buttonID::CONTROLS]);
    optionsVerticalLayout->addSpace(1);
    optionsVerticalLayout->add(mapButtons[buttonID::SOUND]);
    optionsVerticalLayout->addSpace(5);
    mapButtons[buttonID::APPLY_SETTINGS]->disable();
    mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
    optionsVerticalLayout->add(mapButtons[buttonID::APPLY_SETTINGS]);
    optionsVerticalLayout->addSpace(1);
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
                                   if (itemSelected == iter.second)
                                       setCurrentResolution(iter.first);
                               }
                               if (currentResolution !=
                                   machine->configWindow.mapResolutionString[machine->configWindow.currentResolution]) {
                                   mapButtons[buttonID::APPLY_SETTINGS]->enable();
                                   mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(1);
                                   changesMadeVideo = true;
                               } else if ((currentResolution ==
                                           machine->configWindow.mapResolutionString[machine->configWindow.currentResolution]) &&
                                          !checkBoxFullScreenChecked) {
                                   mapButtons[buttonID::APPLY_SETTINGS]->disable();
                                   mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
                                   changesMadeVideo = false;
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
    fullScreenCheck->connect("clicked", [&] {
        checkBoxFullScreenChecked = !checkBoxFullScreenChecked;
        if (!isFullScreen) {
            if (checkBoxFullScreenChecked && changesMadeVideo) {
                mapButtons[buttonID::APPLY_SETTINGS]->enable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(1);
                changesMadeVideo = true;
                std::cout << 1 << std::endl;
            }
            if (checkBoxFullScreenChecked && !changesMadeVideo) {
                mapButtons[buttonID::APPLY_SETTINGS]->enable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(1);
                changesMadeVideo = true;
                std::cout << 2 << std::endl;
            }
            if ((!checkBoxFullScreenChecked && (currentResolution ==
                                                machine->configWindow.mapResolutionString[machine->configWindow.currentResolution])) &&
                !isFullScreen) {
                mapButtons[buttonID::APPLY_SETTINGS]->disable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
                changesMadeVideo = false;
                std::cout << 3 << std::endl;
            }
            if (!checkBoxFullScreenChecked && !changesMadeVideo) {
                mapButtons[buttonID::APPLY_SETTINGS]->disable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
                changesMadeVideo = false;
                std::cout << 4 << std::endl;
            }
        }
        // Is fullscreen
        if (isFullScreen) {

            if (!checkBoxFullScreenChecked && changesMadeVideo) {
                mapButtons[buttonID::APPLY_SETTINGS]->enable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(1);
                changesMadeVideo = true;
                std::cout << 1 << std::endl;
            }
            if (!checkBoxFullScreenChecked && !changesMadeVideo) {
                mapButtons[buttonID::APPLY_SETTINGS]->enable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(1);
                changesMadeVideo = true;
                std::cout << 2 << std::endl;
            }
            if ((checkBoxFullScreenChecked && (currentResolution ==
                                                machine->configWindow.mapResolutionString[machine->configWindow.currentResolution]))) {
                mapButtons[buttonID::APPLY_SETTINGS]->disable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
                changesMadeVideo = false;
                std::cout << 3 << std::endl;
            }
            if (checkBoxFullScreenChecked && !changesMadeVideo) {
                mapButtons[buttonID::APPLY_SETTINGS]->disable();
                mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
                changesMadeVideo = false;
                std::cout << 4 << std::endl;
            }
        }
    });

    // Adds the widgets and layout to the GUI
    videoSettingsLayout->add(resLabel);
    videoSettingsLayout->addSpace();
    videoSettingsLayout->add(resolutionBox);
    videoSettingsLayout->addSpace();
    hori->add(fullScreenCheck);
    hori->addSpace(5);
    videoSettingsLayout->add(hori);
    videoSettingsLayout->addSpace(5);

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
    muteSound->connect("clicked", [&] {
        checkBoxMuteChecked = !checkBoxMuteChecked;
    });

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
    tempSlider->setValue(10);
    mapSliders.emplace(sliderType, tempSlider);
}

void ConfigMenu::defeatScreenLayout() {
    // Label
    tgui::Label::Ptr tempLabel = theme->load("Label");
    // Vertical Layout
    tgui::VerticalLayout::Ptr defeatLayout = tgui::VerticalLayout::create();
    tgui::VerticalLayout::Ptr tempLayout = tgui::VerticalLayout::create();
    tempLayout->setPosition(machine->configWindow.getWindow().getSize().x / 4,
                            machine->configWindow.getWindow().getSize().y / 4);
    tempLayout->setSize(400, 300);

    // Defeat label
    tgui::Label::Ptr defeatLabel = tgui::Label::copy(tempLabel);
    defeatLabel->setText("Defeat!");
    defeatLabel->setPosition(tempLayout->getSize().x / 2, tempLayout->getSize().y / 2);
    defeatLabel->setTextSize(32);

    // Enter name label
    tgui::Label::Ptr nameLabel = tgui::Label::copy(tempLabel);
    nameLabel->setText("Enter name:");
    nameLabel->setTextSize(24);

    // Edit box
    tgui::EditBox::Ptr nameEditBox = theme->load("EditBox");
    nameEditBox->setDefaultText("Enter name...");
    nameEditBox->setMaximumCharacters(20);

    // Return to menu button
    tgui::Button::Ptr returnToMenu = theme->load("Button");
    returnToMenu->setText("Return to menu...");
    returnToMenu->connect("pressed", [&] {
        std::cout << "Hello" << std::endl;
        machine->setCurrentState(StateMachine::stateID::EXIT);
        std::cout << ", World!" << std::endl;
    });


    // Adds every widget to layout map
    tempLayout->add(defeatLabel);
    tempLayout->addSpace(2);
    tempLayout->add(nameLabel);
    tempLayout->add(nameEditBox, "Enter name...");
    tempLayout->addSpace();
    tempLayout->add(returnToMenu);
    mapLayouts.emplace(layouts::DEFEAT, tempLayout);
}

void ConfigMenu::applyChanges() {
    if (changesMadeVideo) {

        currentResolution = machine->configWindow.mapResolutionString[machine->configWindow.currentResolution];
        if (checkBoxFullScreenChecked) {
            machine->configWindow.setWindowResolution(getCurrentResolution(), sf::Style::Fullscreen);
            isFullScreen = true;
        } else {
            machine->configWindow.setWindowResolution(getCurrentResolution(), sf::Style::Default);
            isFullScreen = false;

        }

    }
    if (changesMadeControls) {

    }
    if (changesMadeSound) {

        machine->configSound.setMasterVolume(mapSliders[buttonID::MASTER_SLIDER]->getValue());
        machine->configSound.setMusicVolume(mapSliders[buttonID::MUSIC_SLIDER]->getValue());
        machine->configSound.setSoundEffects(mapSliders[buttonID::EFFECT_SLIDER]->getValue());
    }
    mapButtons[buttonID::APPLY_SETTINGS]->disable();
    mapButtons[buttonID::APPLY_SETTINGS]->setOpacity(0.5f);
    changesMadeVideo = false;
}

void ConfigMenu::checkChanges() {

}

