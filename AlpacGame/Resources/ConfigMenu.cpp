#include "ConfigMenu.h"
#include "../state/StateMachine.h"

void ConfigMenu::run(StateMachine &stateMachine) {

    this->machine = &stateMachine;
    theme = tgui::Theme::create("Resources/MenuUITheme.txt");
    pictureTittleMenu = tgui::Picture::create("Resources/TittleMenuBG.png");
    pictureMenu = tgui::Picture::create("Resources/MenuBG.png");
    masterButton = theme->load("Button");


    createButton(buttonID::RESUME, "Resume", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER); });

    createButton(buttonID::PLAYGAME, "New game", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER); });

    createButton(buttonID::HIGHSCORE, "High score", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->configWindow.getMenuGUI()->add(getPictureMenu());
                     ConfigMenu::loadHighscore("Resources/highscore.txt");
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::HIGHSCORE]);
                 });

    createButton(buttonID::SETTINGS, "Settings", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::OPTION); });

    createButton(buttonID::QUIT, "Quit", "pressed",
                 [&] { machine->setCurrentState(StateMachine::stateID::EXIT); });

    createButton(buttonID::BACK_TO_MAIN, "Back", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->setCurrentState(StateMachine::stateID::MENU);
                 });

    createButton(buttonID::HIGHSCORE, "Highscore", "pressed",
                 [&] {
                     machine->configWindow.getMenuGUI()->removeAllWidgets();
                     machine->configWindow.getMenuGUI()->add(getPictureMenu());
                     ConfigMenu::loadHighscore("Resources/highscore.txt");
                     machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::HIGHSCORE]);
                 });
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

    createButton(buttonID::BACK_HIGHSCORE, "Back", "pressed", [&] {
        machine->configWindow.getMenuGUI()->removeAllWidgets();
        machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::MAINMENU]);
    });


    createSlider(buttonID::MASTER_SLIDER);
    createSlider(buttonID::MUSIC_SLIDER);
    createSlider(buttonID::EFFECT_SLIDER);

    mainMenuLayout(machine->configWindow.getMenuGUI());
}

tgui::Picture::Ptr &ConfigMenu::getPictureMenu() {
    return pictureMenu;
}

tgui::Picture::Ptr &ConfigMenu::getPictureTittle() {
    return pictureTittleMenu;
}

void ConfigMenu::createButton(buttonID buttonID,
                              const std::string &buttonName,
                              const std::string &typeActivation,
                              const std::function<void()> &func) {
    tgui::Button::Ptr tempButton = tgui::Button::copy(masterButton);
    // tempButton->setSize(200, 40);
    tempButton->setText(buttonName);
    tempButton->setTextSize(20);
    tempButton->connect(typeActivation, func);
    mapButtons.emplace(buttonID, tempButton);

}


void ConfigMenu::mainMenuLayout(tgui::Gui *Width) {
    // Creates the different layouts

    // Main menu layout
    machine->configWindow.getMenuGUI()->add(pictureMenu);
    tempVerticalLayout = tgui::VerticalLayout::create();
    tgui::HorizontalLayout::Ptr tempHorizontalLayout = tgui::HorizontalLayout::create();
    pictureTittleMenu->setSize(tgui::bindMax(800, windowWidth), tgui::bindMax(600, windowHeight));
    pictureMenu->setSize(tgui::bindMax(800, windowWidth), tgui::bindMax(600, windowHeight));
    tempVerticalLayout->removeAllWidgets();
    windowWidth = tgui::bindWidth(*Width);
    windowHeight = tgui::bindHeight(*Width);
    tempVerticalLayout->setSize(windowWidth / 2, windowHeight * 5 / 6);
    tempVerticalLayout->setPosition(windowWidth / 4, windowHeight / 6);

    // TODO: add a new font for titleLabel.

    //tempVerticalLayout->add(pictureTittle2);
    tempVerticalLayout->addSpace(2.f);
    tempVerticalLayout->add(mapButtons[buttonID::RESUME]);
    tempVerticalLayout->addSpace(0.1f);
    tempVerticalLayout->add(mapButtons[buttonID::PLAYGAME]);
    tempVerticalLayout->addSpace(0.1f);
    tempVerticalLayout->add(mapButtons[buttonID::HIGHSCORE]);
    tempVerticalLayout->addSpace(0.1f);
    tempVerticalLayout->add(mapButtons[buttonID::SETTINGS]);
    tempVerticalLayout->addSpace(0.1f);
    tempVerticalLayout->add(mapButtons[buttonID::QUIT]);
    tempVerticalLayout->addSpace(1.f);
    tempVerticalLayout->setRatio(0, 7.5);

    // Adds the main menu layout to the map
    mapLayouts.emplace(layouts::MAINMENU, tempVerticalLayout);

    // Settings layout
    tgui::VerticalLayout::Ptr optionsVerticalLayout = tgui::VerticalLayout::create();
    optionsVerticalLayout->setSize(windowWidth / 3, windowHeight);
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
    defeatScreenLayout();
    highscoreLayout();
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
    resLabel->setTextColor(sf::Color::White);


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

    // Layout setup
    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    tgui::HorizontalLayout::Ptr horiLayout = tgui::HorizontalLayout::create();
    layout->setSize(windowWidth / 4, windowHeight / 2);
    layout->setPosition(windowWidth / 3, windowHeight / 6);

    // Slider master label
    tgui::Label::Ptr masterLabel = tgui::Label::create();
    masterLabel->setText("Master volume");
    masterLabel->setTextColor(sf::Color::White);

    // Scrollbar music label
    tgui::Label::Ptr musicLabel = tgui::Label::create();
    musicLabel->setText("Music volume");
    musicLabel->setTextColor(sf::Color::White);

    // Slider SFX label
    tgui::Label::Ptr sfxLabel = tgui::Label::create();
    sfxLabel->setText("Sound effects");
    sfxLabel->setTextColor(sf::Color::White);

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
    horiLayout->addSpace(5.f);
    layout->add(horiLayout);
    layout->addSpace(2.f);
    layout->add(masterLabel);
    layout->addSpace(0.5f);
    layout->add(mapSliders[buttonID::MASTER_SLIDER]);
    layout->addSpace();
    layout->add(musicLabel);
    layout->addSpace(0.5f);
    layout->add(mapSliders[buttonID::MUSIC_SLIDER]);
    layout->addSpace();
    layout->add(sfxLabel);
    layout->addSpace(0.5f);
    layout->add(mapSliders[buttonID::EFFECT_SLIDER]);

    // Emplaces the soundlayout to the layout map
    mapLayouts.emplace(layouts::SOUND, layout);
}


void ConfigMenu::createSlider(buttonID sliderType) {
    // Temporary slider
    tgui::Slider::Ptr tempSlider = theme->load("Slider");
    tempSlider->setValue(10);
    mapSliders.emplace(sliderType, tempSlider);
}


void ConfigMenu::highscoreLayout() {

    // Temporary layout
    tgui::HorizontalLayout::Ptr tempHighscoreLayout = tgui::HorizontalLayout::create();
    tgui::HorizontalLayout::Ptr tempHorizontalLayout = tgui::HorizontalLayout::create();
    tgui::HorizontalLayout::Ptr tempBackButtonLayout = tgui::HorizontalLayout::create();
    tgui::VerticalLayout::Ptr tempVerticalLayout = tgui::VerticalLayout::create();

    // The text box which contains all the highscores
    textBoxHighscore = theme->load("TextBox");
    textBoxHighscore->setReadOnly(true);
    textBoxHighscore->setTextSize(24);

    // The setup for the layout containing the different labels
    tgui::HorizontalLayout::Ptr labelsLayout = tgui::HorizontalLayout::create();
    tempVerticalLayout->setSize(windowWidth, windowHeight);
    tempVerticalLayout->setPosition(0, 0);

    // The setup for the highscore title
    tgui::Label::Ptr highscoreTitle = tgui::Label::create();
    highscoreTitle->setText("Highscore!");
    highscoreTitle->setTextSize(42);

    // Rank label
    tgui::Label::Ptr noLabel = tgui::Label::create();
    noLabel->setText("Rank:");
    noLabel->setTextSize(20);

    // Name label
    tgui::Label::Ptr namelabel = tgui::Label::create();
    namelabel->setText("Name:");
    namelabel->setTextSize(20);

    // Score label
    tgui::Label::Ptr scoreLabel = tgui::Label::create();
    scoreLabel->setText("Score:");
    scoreLabel->setTextSize(20);

    // Back button setup
    tgui::Button::Ptr backButton = tgui::Button::copy(masterButton);
    backButton->setText("Back");
    backButton->connect("pressed", [&] {
        machine->configWindow.getMenuGUI()->removeAllWidgets();
        machine->configWindow.getWindow().clear();
        machine->configWindow.getMenuGUI()->add(pictureMenu);
        machine->configWindow.getMenuGUI()->add(mapLayouts[layouts::MAINMENU]);
    });

    // Horizontal layout which contains the back button
    tempBackButtonLayout->addSpace(2);
    tempBackButtonLayout->add(backButton);
    tempBackButtonLayout->addSpace(2);

    // Horizontal layout which contains the text box
    tempHorizontalLayout->addSpace(0.125f);
    tempHorizontalLayout->add(textBoxHighscore);
    tempHorizontalLayout->addSpace(0.125f);

    // Horizontal layout which contains the different labels
    labelsLayout->addSpace(0.60f);
    labelsLayout->add(noLabel);
    labelsLayout->add(namelabel);
    labelsLayout->addSpace(1);
    labelsLayout->add(scoreLabel);
    labelsLayout->addSpace(1);

    // Horizontal layout which contains the highscore title
    tempHighscoreLayout->addSpace(0.3333f);
    tempHighscoreLayout->add(highscoreTitle);

    // Vertical layout which contains all of the widgets and layouts
    // and adds the layout to a map.
    tempVerticalLayout->addSpace(0.5f);
    tempVerticalLayout->add(tempHighscoreLayout);
    tempVerticalLayout->add(labelsLayout);
    tempVerticalLayout->add(tempHorizontalLayout);
    tempVerticalLayout->addSpace();
    tempVerticalLayout->add(tempBackButtonLayout);
    tempVerticalLayout->addSpace(0.5f);
    tempVerticalLayout->setRatio(3, 5);
    tempVerticalLayout->setRatio(5, 0.5);

    mapLayouts.emplace(layouts::HIGHSCORE, tempVerticalLayout);
}

void ConfigMenu::loadHighscore(std::string highScoreFile) {
    rank = 1;
    textBoxHighscore->setText("");
    file.open(highScoreFile);
    //Loads the names and scores from a file and prints them in a text box
    if (file.is_open()) {
        while (std::getline(file, line)) {
            // First reads name, appens a #rank to that name
            if (alternateHighscoreWindow) {
                // rankString is what the rank to the player highscore is #1-10
                rankString = std::to_string(rank) + ".";
                // Appends spaces to the string so that the alignment of the highscore text looks nice.
                if (rank == 10) {
                    rankString.append(6, ' ');
                } else {
                    rankString.append(7, ' ');
                }
                spaces = 9 - (int) line.size();
                // Inserts the #rank of the player in the string
                line.insert(0, rankString);
                line.append(spaces, ' ');
                tempString = line;

                // This boolean is required because of how the format of the highscore.txt file is formatted,
                // first line is name, second line is the highscore to that name,
                // third line is name, fourth line is score, and so on...
                alternateHighscoreWindow = !alternateHighscoreWindow;
                // rank increment
                rank++;
                // Appends the score to the name
            } else {
                scoreSpaces = 20 - (int) line.size();
                // Appends spaces to the string so that the alignment of the highscore text looks nice.
                tempString.append(" ");
                tempString.append(scoreSpaces, ' ');
                if (rank == 11) {
                    tempString.append(line);
                } else {
                    tempString.append(line + "\n");
                }
                // Adds the formatted string to the text box
                textBoxHighscore->addText(tempString);
                alternateHighscoreWindow = !alternateHighscoreWindow;
            }
        }
        file.close();
    } else {
        std::cout << "COULD NOT READ FILE!" << std::endl;
    }
}

ConfigWindow::Resolution ConfigMenu::getCurrentResolution() const {
    return machine->configWindow.currentResolution;
}

void ConfigMenu::setCurrentResolution(ConfigWindow::Resolution currentResolution) {
    machine->configWindow.currentResolution = currentResolution;
}

void ConfigMenu::defeatScreenLayout() {
    // Label
    tgui::Label::Ptr tempLabel = theme->load("Label");
    // Vertical Layout
    tgui::VerticalLayout::Ptr defeatLayout = tgui::VerticalLayout::create();
    tgui::VerticalLayout::Ptr tempLayout = tgui::VerticalLayout::create();
    tgui::HorizontalLayout::Ptr defeatTitleLayout = tgui::HorizontalLayout::create();
    tempLayout->setPosition(machine->configWindow.getWindow().getSize().x / 4,
                            machine->configWindow.getWindow().getSize().y / 4);
    tempLayout->setSize(400, 300);

    // Defeat label
    tgui::Label::Ptr defeatLabel = tgui::Label::copy(tempLabel);
    defeatLabel->setText("DEFEAT!");
    defeatLabel->setPosition(tempLayout->getSize().x / 2, tempLayout->getSize().y / 2);
    defeatLabel->setTextSize(34);
    defeatLabel->setTextColor(tgui::Color(255, 10, 10));

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
        machine->setCurrentState(StateMachine::stateID::EXIT);

    });


    defeatTitleLayout->addSpace();
    defeatTitleLayout->add(defeatLabel);
    defeatTitleLayout->addSpace();


    // Adds every widget to layout map
    tempLayout->add(defeatTitleLayout);
    tempLayout->addSpace();
    tempLayout->add(nameLabel);
    tempLayout->add(nameEditBox, "Enter name...");
    tempLayout->addSpace(0.5);
    tempLayout->add(returnToMenu);
    tempLayout->setRatio(0, 3);

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
