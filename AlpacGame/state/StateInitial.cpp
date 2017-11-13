
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
    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::VIDEO, "Video", "pressed", [&] {
        stateMachine.configWindow.getMenuGUI()->removeAllWidgets();
        stateMachine.configWindow.getMenuGUI()->add(stateMachine.configMenu.getPictureMenu());
        stateMachine.configWindow.getMenuGUI()->add(
                stateMachine.configMenu.mapLayouts.find(ConfigMenu::MAP_LAYOUTS::SETTINGS)->second);
        stateMachine.configWindow.getMenuGUI()->add(stateMachine.configMenu.mapLayouts.find(
                ConfigMenu::MAP_LAYOUTS::VIDEO)->second);
    });
    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::CONTROLS, "Controls", "pressed",
                                         [&] { std::cout << "Control settings" << std::endl; });
    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::SOUND, "Sound", "pressed", [&] {
        stateMachine.configWindow.getMenuGUI()->removeAllWidgets();
        stateMachine.configWindow.getMenuGUI()->add(stateMachine.configMenu.getPictureMenu());
        stateMachine.configWindow.getMenuGUI()->add(stateMachine.configMenu.mapLayouts.find(
                ConfigMenu::MAP_LAYOUTS::SETTINGS)->second);
        stateMachine.configWindow.getMenuGUI()->add(
                stateMachine.configMenu.mapLayouts.find(ConfigMenu::MAP_LAYOUTS::SOUND)->second);
        std::cout << " Sound settings" << std::endl;
    });
    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::BACK_TO_MAIN, "Back", "pressed",
                                         [&] { stateMachine.setCurrentState(StateMachine::stateID::MENU); });
    stateMachine.configMenu.createButton(ConfigMenu::MAP_BUTTONS::APPLY, "Apply changes", "pressed",
                                         [&] { std::cout << "Changes applied" << std::endl; });
    stateMachine.configMenu.createLayouts(stateMachine.configWindow.getMenuGUI());
    stateMachine.configGame.run(stateMachine.configWindow.getWindow());

    stateMachine.setCurrentState(StateMachine::stateID::MENU);
}
