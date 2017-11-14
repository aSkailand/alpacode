#include "ConfigMenu.h"
#include "../state/StateMachine.h"

void ConfigMenu::run(StateMachine &stateMachine) {

    this->machine = &stateMachine;

    theme = tgui::Theme::create("Resources/BabyBlue.txt");
    pictureMenu = tgui::Picture::create("Resources/aluminium.jpg");
    masterButton = theme->load("Button");

    createButton(buttons::PLAYGAME, "Play game!", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER); });

    createButton(buttons::SETTINGS, "Settings", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::OPTION); });

    createButton(buttons::QUIT, "Quit", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::EXIT); });

    createButton(buttons::BACK_TO_MAIN, "Back", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::MENU); });

    createButton(buttons::VIDEO, "Video", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->configWindow.getMenuGUI()->add(getPictureMenu());
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::SETTINGS]);
                     machine->configWindow.getMenuGUI()->add(mapLayouts[ConfigMenu::layouts::VIDEO]);
                 });

    createButton(buttons::CONTROLS, "Controls", "pressed",
                 [&] {});

    createButton(buttons::SOUND, "Sound", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->configWindow.getMenuGUI()->add(getPictureMenu());
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::SETTINGS]);
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::SOUND]);

                 });


    createButton(buttons::APPLY_VIDEO_SETTINGS, "Apply changes", "pressed",
                 [&] {
                     if (checkBoxFullScreenChecked) {
                         machine->configWindow.setWindowResolution(getCurrentResolution(), sf::Style::Fullscreen);
                     } else {
                         machine->configWindow.setWindowResolution(getCurrentResolution(), sf::Style::Default);
                     }
                 });

    createButton(buttons::APPLY_SOUND_SETTINGS, "Apply changes", "pressed",
                 [&] {
                     machine->configSound.setMasterVolume(mapSliders[buttons::MASTER_SLIDER]->getValue());
                     machine->configSound.setMusicVolume(mapSliders[buttons::MUSIC_SLIDER]->getValue());
                     machine->configSound.setSoundEffects(mapSliders[buttons::EFFECT_SLIDER]->getValue());
                 });

    createSlider(buttons::MASTER_SLIDER);
    createSlider(buttons::MUSIC_SLIDER);
    createSlider(buttons::EFFECT_SLIDER);

    mainMenuLayout(machine->configWindow.getMenuGUI());
}

tgui::Picture::Ptr &ConfigMenu::getPictureMenu() {
    return pictureMenu;
}

void ConfigMenu::createButton(buttons buttonType,
                              const std::string &buttonName,
                              const std::string &typeActivation,
                              const std::function<void()> &func) {
    tgui::Button::Ptr tempButton = tgui::Button::copy(masterButton);
    tempButton->setText(buttonName);
    tempButton->connect(typeActivation, func);
    mapButtons.emplace(buttonType, tempButton);
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

    tempVerticalLayout->add(mapButtons[buttons::PLAYGAME]);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(mapButtons[buttons::SETTINGS]);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(mapButtons[buttons::QUIT]);

    // Adds the main menu layout to the map
    mapLayouts.emplace(layouts::MAINMENU, tempVerticalLayout);

    // Settings layout
    tgui::VerticalLayout::Ptr optionsVerticalLayout = tgui::VerticalLayout::create();
    optionsVerticalLayout->setSize(windowWidth / 4, windowHeight);
    optionsVerticalLayout->setPosition(windowWidth - (windowWidth - 10), 0);
    optionsVerticalLayout->removeAllWidgets();
    optionsVerticalLayout->addSpace(0.5f);
    optionsVerticalLayout->add(mapButtons[buttons::VIDEO]);
    optionsVerticalLayout->addSpace(2);
    optionsVerticalLayout->add(mapButtons[buttons::CONTROLS]);
    optionsVerticalLayout->addSpace(2);
    optionsVerticalLayout->add(mapButtons[buttons::SOUND]);
    optionsVerticalLayout->addSpace(6);
    optionsVerticalLayout->add(mapButtons[buttons::BACK_TO_MAIN]);
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
    for (auto &iter : machine->configWindow.mapResolution) {
        resolutionBox->addItem(iter.second);
    }

    // Set initial selection
    resolutionBox->setSelectedItem(machine->configWindow.mapResolution[machine->configWindow.currentResolution]);

    // Setup selection mechanism
    resolutionBox->connect("ItemSelected",
                           [&](std::string itemSelected) {
                               for (auto &iter : machine->configWindow.mapResolution) {
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
    videoSettingsLayout->add(mapButtons[buttons::APPLY_VIDEO_SETTINGS]);

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
    layout->add(mapSliders[buttons::MASTER_SLIDER]);
    layout->addSpace();
    layout->add(musicLabel);
    layout->add(mapSliders[buttons::MUSIC_SLIDER]);
    layout->addSpace();
    layout->add(sfxLabel);
    layout->add(mapSliders[buttons::EFFECT_SLIDER]);
    layout->addSpace();
    layout->add(mapButtons[buttons::APPLY_SOUND_SETTINGS]);

    mapLayouts.emplace(layouts::SOUND, layout);
}

ConfigWindow::Resolution ConfigMenu::getCurrentResolution() const {
    return machine->configWindow.currentResolution;
}

void ConfigMenu::setCurrentResolution(ConfigWindow::Resolution currentResolution) {
    machine->configWindow.currentResolution = currentResolution;
}

void ConfigMenu::createSlider(buttons sliderType) {
    // Temporary slider
    tgui::Slider::Ptr tempSlider = theme->load("Slider");
    mapSliders.emplace(sliderType, tempSlider);
}

