#include "ConfigMenu.h"

void ConfigMenu::run() {
    theme = tgui::Theme::create("Resources/BabyBlue.txt");
    pictureMenu = tgui::Picture::create("Resources/aluminium.jpg");
    masterButton = theme->load("Button");
}

tgui::Picture::Ptr &ConfigMenu::getPictureMenu() {
    return pictureMenu;
}

void ConfigMenu::createButton(ConfigMenu::MAP_BUTTONS buttonType, std::string buttonName, std::string typeActivation,
                              std::function<void()> func) {
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

    tempVerticalLayout->add(mapButtons.find(MAP_BUTTONS::PLAYGAME)->second);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(mapButtons.find(MAP_BUTTONS::SETTINGS)->second);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(mapButtons.find(MAP_BUTTONS::QUIT)->second);

    // Adds the main menu layout to the map
    mapLayouts.emplace(MAP_LAYOUTS::MAINMENU, tempVerticalLayout);

    // Settings layout
    tgui::VerticalLayout::Ptr optionsVerticalLayout = tgui::VerticalLayout::create();
    optionsVerticalLayout->setSize(windowWidth / 4, windowHeight);
    optionsVerticalLayout->setPosition(windowWidth - (windowWidth - 10), 0);
    optionsVerticalLayout->removeAllWidgets();
    optionsVerticalLayout->addSpace(0.5f);
    optionsVerticalLayout->add(mapButtons.find(MAP_BUTTONS::VIDEO)->second);
    optionsVerticalLayout->addSpace(2);
    optionsVerticalLayout->add(mapButtons.find(MAP_BUTTONS::CONTROLS)->second);
    optionsVerticalLayout->addSpace(2);
    optionsVerticalLayout->add(mapButtons.find(MAP_BUTTONS::SOUND)->second);
    optionsVerticalLayout->addSpace(6);
    optionsVerticalLayout->add(mapButtons.find(MAP_BUTTONS::BACK_TO_MAIN)->second);
    optionsVerticalLayout->addSpace(0.5f);

    // Adds the settings layout to the map
    mapLayouts.emplace(MAP_LAYOUTS::SETTINGS, optionsVerticalLayout);

    videoSettingsLayout();
    soundSettingsLayout();
}


void ConfigMenu::videoSettingsLayout() {

    tgui::VerticalLayout::Ptr videoSettingsLayout = tgui::VerticalLayout::create();
    tgui::HorizontalLayout::Ptr hori = tgui::HorizontalLayout::create();
    videoSettingsLayout->setSize(windowWidth / 4, windowHeight / 2);
    videoSettingsLayout->setPosition(windowWidth / 3, windowHeight / 6);

    // ComboBox
    tgui::ComboBox::Ptr resolutonBox = theme->load("ComboBox");
    resolutonBox->addItem("800x600");
    resolutonBox->addItem("1280x720");
    resolutonBox->addItem("1920x1080");
    resolutonBox->setSelectedItem("800x600");
    resolutonBox->connect("ItemSelected",
                          [&](std::string itemSelected) {
                              if (itemSelected == "800x600") {
                                  setCurrentResolution(RESOLUTION::RES800x600);
                              } else if (itemSelected == "1280x720") {
                                  setCurrentResolution(RESOLUTION::RES1280x720);
                              } else if (itemSelected == "1920x1080") {
                                  setCurrentResolution(RESOLUTION::RES1920x1080);
                              }
                              std::cout << (int) getCurrentResolution() << std::endl;
                          });

    // Resoulution label
    tgui::Label::Ptr resLabel = tgui::Label::create();
    resLabel->setText("Resoulution");
    resLabel->setTextSize(30);
    resLabel->setSize(100, 50);


    // Fullscreen checkbox
    tgui::CheckBox::Ptr fullScreenCheck = theme->load("CheckBox");
    fullScreenCheck->setPosition(windowWidth / 6, windowHeight / 2.5);
    fullScreenCheck->setText("Fullscreen");
    fullScreenCheck->setTextSize(24);
    fullScreenCheck->connect("clicked", [&] {
        checkBoxFullscreenChecked = !checkBoxFullscreenChecked;
        std::cout << checkBoxFullscreenChecked << std::endl;
        std::cout << (int) getCurrentResolution() << std::endl;
    });

    // Adds the widgets and layout to the GUI
    videoSettingsLayout->add(resLabel);
    videoSettingsLayout->addSpace();
    videoSettingsLayout->add(resolutonBox);
    videoSettingsLayout->addSpace();
    hori->add(fullScreenCheck);
    hori->addSpace(5);
    videoSettingsLayout->add(hori);
    videoSettingsLayout->addSpace(5);
    videoSettingsLayout->add(mapButtons.find(ConfigMenu::MAP_BUTTONS::APPLY_VIDEO_SETTINGS)->second);

    mapLayouts.emplace(MAP_LAYOUTS::VIDEO, videoSettingsLayout);
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
    layout->add(mapSliders.find(MAP_BUTTONS::MASTER_SLIDER)->second);
    layout->addSpace();
    layout->add(musicLabel);
    layout->add(mapSliders.find(MAP_BUTTONS::MUSIC_SLIDER)->second);
    layout->addSpace();
    layout->add(sfxLabel);
    layout->add(mapSliders.find(MAP_BUTTONS::EFFECT_SLIDER)->second);
    layout->addSpace();
    layout->add(mapButtons.find(MAP_BUTTONS::APPLY_SOUND_SETTINGS)->second);
    mapLayouts.emplace(MAP_LAYOUTS::SOUND, layout);
}

ConfigMenu::RESOLUTION ConfigMenu::getCurrentResolution() const {
    return currentResolution;
}

void ConfigMenu::setCurrentResolution(ConfigMenu::RESOLUTION currentResolution) {
    ConfigMenu::currentResolution = currentResolution;
}

void ConfigMenu::createSlider(ConfigMenu::MAP_BUTTONS sliderType) {
    // Temporary slider
    tgui::Slider::Ptr tempSlider = theme->load("Slider");
    mapSliders.emplace(sliderType, tempSlider);
}

