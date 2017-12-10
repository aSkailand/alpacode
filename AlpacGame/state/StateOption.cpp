#include "StateOption.h"

void StateOption::goNext(StateMachine &stateMachine) {

    /// Assigning pointers

    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    menuGUI = machine->configWindow.getMenuGUI();
    configGame = &machine->configGame;

    /// TGUI setup
    menuGUI->removeAllWidgets();
    menuGUI->add(machine->configMenu->getPictureMenu());
    menuGUI->add(machine->configMenu->mapLayouts[ConfigMenu::layouts::SETTINGS]);
    menuGUI->add(machine->configMenu->mapLayouts[ConfigMenu::layouts::VIDEO]);

    while (PollOption()) {
        drawOption();
    }
}

bool StateOption::PollOption() {
    sf::Event event{};
    while(window->pollEvent(event)){

        menuGUI->handleEvent(event);

        if (event.type == sf::Event::Closed) {
            std::cout << "Apple" << std::endl;
            machine->setCurrentState(StateMachine::stateID::EXIT);
        }

        checkIfAnyControlAssigningKeyIsPressed(event);

    }


    return machine->getCurrentState() == StateMachine::stateID::OPTION;
}

void StateOption::drawOption() {
    menuGUI->draw();
    window->display();
}

void StateOption::KeyBinding(sf::Keyboard::Key newKey, std::string keyString) {


    for (auto &MapControlKey : configGame->MapControlKeys) {
        if (newKey == MapControlKey.second) {
            break;
        }
    }
    configGame->buttonToAssign->setText(keyString);
    configGame->MapControlKeys[configGame->controlToAssign] = newKey;
    configGame->controlToAssign = ConfigGame::ControlName::NOTHING;
    configGame->buttonToAssign = nullptr;

}

void StateOption::checkIfAnyControlAssigningKeyIsPressed(sf::Event event) {
    if (configGame->controlToAssign != ConfigGame::ControlName::NOTHING && event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {

            case sf::Keyboard::Escape:
                break;
            case sf::Keyboard::A: {
                KeyBinding(sf::Keyboard::A, "A");
                break;
            }
            case sf::Keyboard::B: {
                KeyBinding(sf::Keyboard::B, "B");
                break;
            }
            case sf::Keyboard::C:{
                KeyBinding(sf::Keyboard::C, "C");
                break;
            }
            case sf::Keyboard::D:{
                KeyBinding(sf::Keyboard::D, "D");
                break;
            }
            case sf::Keyboard::E:{
                KeyBinding(sf::Keyboard::E, "E");
                break;
            }
            case sf::Keyboard::F:{
                KeyBinding(sf::Keyboard::F, "F");
                break;
            }
            case sf::Keyboard::G:{
                KeyBinding(sf::Keyboard::G, "G");
                break;
            }
            case sf::Keyboard::H:{
                KeyBinding(sf::Keyboard::H, "H");
                break;
            }
            case sf::Keyboard::I:{
                KeyBinding(sf::Keyboard::I, "I");
                break;
            }
            case sf::Keyboard::J:{
                KeyBinding(sf::Keyboard::J, "J");
                break;
            }
            case sf::Keyboard::K:{
                KeyBinding(sf::Keyboard::K, "K");
                break;
            }
            case sf::Keyboard::L:{
                KeyBinding(sf::Keyboard::L, "L");
                break;
            }
            case sf::Keyboard::M:{
                KeyBinding(sf::Keyboard::M, "M");
                break;
            }
            case sf::Keyboard::N:{
                KeyBinding(sf::Keyboard::N, "N");
                break;
            }
            case sf::Keyboard::O:{
                KeyBinding(sf::Keyboard::O, "O");
                break;
            }
            case sf::Keyboard::P:{
                KeyBinding(sf::Keyboard::P, "P");
                break;
            }
            case sf::Keyboard::Q:{
                KeyBinding(sf::Keyboard::Q, "Q");
                break;
            }
            case sf::Keyboard::R:{
                KeyBinding(sf::Keyboard::R, "R");
                break;
            }
            case sf::Keyboard::S:{
                KeyBinding(sf::Keyboard::S, "S");
                break;
            }
            case sf::Keyboard::T:{
                KeyBinding(sf::Keyboard::T, "T");
                break;
            }
            case sf::Keyboard::U:{
                KeyBinding(sf::Keyboard::U, "U");
                break;
            }
            case sf::Keyboard::V:{
                KeyBinding(sf::Keyboard::V, "V");
                break;
            }
            case sf::Keyboard::W:{
                KeyBinding(sf::Keyboard::W, "W");
                break;
            }
            case sf::Keyboard::X:{
                KeyBinding(sf::Keyboard::X, "X");
                break;
            }
            case sf::Keyboard::Y:{
                KeyBinding(sf::Keyboard::Y, "Y");
                break;
            }
            case sf::Keyboard::Z:{
                KeyBinding(sf::Keyboard::Z, "Z");
                break;
            }
            case sf::Keyboard::Num0:{
                KeyBinding(sf::Keyboard::Num0,"0");
                break;
            }
            case sf::Keyboard::Num1:{
                KeyBinding(sf::Keyboard::Num1,"1");
                break;
            }
            case sf::Keyboard::Num2:{
                KeyBinding(sf::Keyboard::Num2, "2");
                break;
            }
            case sf::Keyboard::Num3:{
                KeyBinding(sf::Keyboard::Num3, "3");
                break;
            }
            case sf::Keyboard::Num4:{
                KeyBinding(sf::Keyboard::Num4, "4");
                break;
            }
            case sf::Keyboard::Num5:{
                KeyBinding(sf::Keyboard::Num5, "5");
                break;
            }
            case sf::Keyboard::Num6:{
                KeyBinding(sf::Keyboard::Num6, "6");
                break;
            }
            case sf::Keyboard::Num7:{
                KeyBinding(sf::Keyboard::Num7, "7");
                break;
            }
            case sf::Keyboard::Num8:{
                KeyBinding(sf::Keyboard::Num8, "8");
                break;
            }
            case sf::Keyboard::Num9:{
                KeyBinding(sf::Keyboard::Num9, "9");
                break;
            }
            case sf::Keyboard::LControl:{
                KeyBinding(sf::Keyboard::LControl, "LCTRL");
                break;
            }
            case sf::Keyboard::LShift:{
                KeyBinding(sf::Keyboard::LShift, "LSHIFT");
                break;
            }
            case sf::Keyboard::LAlt:{
                KeyBinding(sf::Keyboard::LAlt, "LALT");
                break;
            }
            case sf::Keyboard::RControl:{
                KeyBinding(sf::Keyboard::RControl, "RCTRL");
                break;
            }
            case sf::Keyboard::RShift:{
                KeyBinding(sf::Keyboard::RShift, "RCTRL");
                break;
            }
            case sf::Keyboard::RAlt:{
                KeyBinding(sf::Keyboard::RAlt, "RALT");
                break;
            }
            case sf::Keyboard::Comma:{
                KeyBinding(sf::Keyboard::Comma, ",");
                break;
            }
            case sf::Keyboard::Period:{
                KeyBinding(sf::Keyboard::Period, ".");
                break;
            }
            case sf::Keyboard::Quote:{
                KeyBinding(sf::Keyboard::Quote, "'");
                break;
            }
            case sf::Keyboard::Dash:{
                KeyBinding(sf::Keyboard::Dash, "-");
                break;
            }
            case sf::Keyboard::Space:{
                KeyBinding(sf::Keyboard::Space, "Space");
                break;
            }
            case sf::Keyboard::Tab:{
                KeyBinding(sf::Keyboard::Tab, "TAB");
                break;
            }
            case sf::Keyboard::Left:{
                KeyBinding(sf::Keyboard::Left, "LEFT");
                break;
            }
            case sf::Keyboard::Right:{
                KeyBinding(sf::Keyboard::Right, "RIGHT");
                break;
            }
            case sf::Keyboard::Up:{
                KeyBinding(sf::Keyboard::Up, "UP");
                break;
            }
            case sf::Keyboard::Down:{
                KeyBinding(sf::Keyboard::Down, "DOWN");
                break;
            }
            default: break;
        }

    }
}

