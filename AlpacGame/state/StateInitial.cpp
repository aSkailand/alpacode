
#include "StateInitial.h"
#include "StateMachine.h"

void StateInitial::goNext(StateMachine &stateMachine) {

    stateMachine.configWindow.run();
    stateMachine.configMenu.run();
    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::PLAYGAME, "Play game!", "pressed",
                                         [&] { stateMachine.setCurrentState(StateMachine::stateID::SINGLEPLAYER); });

    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::SETTINGS, "Settings", "pressed",
                                         [&] { stateMachine.setCurrentState(StateMachine::stateID::OPTION); });

    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::QUIT, "Quit", "pressed",
                                         [&] { stateMachine.setCurrentState(StateMachine::stateID::EXIT); });

    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::BACK_TO_MAIN, "Back", "pressed",
                                         [&] { stateMachine.setCurrentState(StateMachine::stateID::MENU); });


    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::VIDEO, "Video", "pressed",
                                         [&] {
                                             stateMachine.configWindow.getMenuGUI()->removeAllWidgets();
                                             stateMachine.configWindow.getMenuGUI()->add(
                                                     stateMachine.configMenu.getPictureMenu());
                                             stateMachine.configWindow.getMenuGUI()->add(
                                                     stateMachine.configMenu.mapLayouts.find(
                                                             ConfigMenu::MAP_LAYOUTS::SETTINGS)->second);
                                             stateMachine.configWindow.getMenuGUI()->add(
                                                     stateMachine.configMenu.mapLayouts.find(
                                                             ConfigMenu::MAP_LAYOUTS::VIDEO)->second);
                                         });

    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::CONTROLS, "Controls", "pressed",
                                         [&] {});

    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::SOUND, "Sound", "pressed",
                                         [&] {
                                             stateMachine.configWindow.getMenuGUI()->removeAllWidgets();
                                             stateMachine.configWindow.getMenuGUI()->add(
                                                     stateMachine.configMenu.getPictureMenu());
                                             stateMachine.configWindow.getMenuGUI()->add(
                                                     stateMachine.configMenu.mapLayouts.find(
                                                             ConfigMenu::MAP_LAYOUTS::SETTINGS)->second);
                                             stateMachine.configWindow.getMenuGUI()->add(
                                                     stateMachine.configMenu.mapLayouts.find(
                                                             ConfigMenu::MAP_LAYOUTS::SOUND)->second);

                                         });


    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::APPLY_VIDEO_SETTINGS, "Apply changes", "pressed",
                                         [&] {
                                             if (stateMachine.configMenu.checkBoxFullscreenChecked) {
                                                 stateMachine.configWindow.setFullScreenWindowResolution(
                                                         (int) stateMachine.configMenu.getCurrentResolution());
                                             } else {
                                                 stateMachine.configWindow.setWindowResolution(
                                                         (int) stateMachine.configMenu.getCurrentResolution());
                                             }
                                         });

    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::APPLY_SOUND_SETTINGS, "Apply changes", "pressed",
                                         [&] {stateMachine.configSound.setMasterVolume(stateMachine.configMenu.mapSliders.find(ConfigMenu::MAP_BUTTONS::MASTER_SLIDER)->second->getValue());
                                             stateMachine.configSound.setMusicVolume(stateMachine.configMenu.mapSliders.find(ConfigMenu::MAP_BUTTONS::MUSIC_SLIDER)->second->getValue());
                                             stateMachine.configSound.setSoundEffects(stateMachine.configMenu.mapSliders.find(ConfigMenu::MAP_BUTTONS::EFFECT_SLIDER)->second->getValue());
                                         });
    stateMachine.configMenu.createSlider(ConfigMenu::MAP_BUTTONS::MASTER_SLIDER);
    stateMachine.configMenu.createSlider(ConfigMenu::MAP_BUTTONS::MUSIC_SLIDER);
    stateMachine.configMenu.createSlider(ConfigMenu::MAP_BUTTONS::EFFECT_SLIDER);

    stateMachine.configMenu.mainMenuLayout(stateMachine.configWindow.getMenuGUI());
    stateMachine.configGame.run(stateMachine.configWindow.getWindow());

    stateMachine.setCurrentState(StateMachine::stateID::MENU);
}
