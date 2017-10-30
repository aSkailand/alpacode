#include "StateMenu.h"


void StateMenu::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    window->setView(sf::View(window->getDefaultView()));
    menuGUI = &machine->configWindow.getMenuGUI();
    menuGUI->removeAllWidgets();


    //TODO: Make theme and layout pointers?
    // Creates a theme
    tgui::Theme::Ptr theme = tgui::Theme::create("C:/dev/libs/TGUI/include/TGUI/widgets/Black.txt");

    // Creates a layout
    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    layout->setSize(400,400);
    layout->setPosition(machine->configWindow.getScreenWidth() / 4, machine->configWindow.getScreenHeight() /4);


    // Creates a button
    tgui::Button::Ptr button = theme->load("Button");
    button->setText("Play Game!");

    tgui::Button::Ptr button1 = tgui::Button::copy(button);
    button1->setText("Settings");

    tgui::Button::Ptr button2 = tgui::Button::copy(button);
    button2->setText("Quit");

    button->connect("pressed", [this] {machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER); std::cout << machine->configWindow.getScreenHeight() << ", "  << machine->configWindow.getScreenWidth()  << std::endl;});
    button1->connect("pressed", [this] {machine->setCurrentState(StateMachine::stateID::OPTION);});
    button2->connect("pressed", [this] {machine->setCurrentState(StateMachine::stateID::EXIT);});

    layout->add(button);
    layout->addSpace();
    layout->add(button1);
    layout->addSpace();
    layout->add(button2);
    layout->addSpace();
    menuGUI->add(layout);


    while (pollMenu()) {
        drawMenu();
    }
}

bool StateMenu::pollMenu() {
    sf::Event event;
    window->pollEvent(event);

    if(event.type== sf::Event::Closed) window->close();

    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::MENU;
}

void StateMenu::drawMenu() {
    window->clear(sf::Color::Black);
    menuGUI->draw();
    window->display();
}
