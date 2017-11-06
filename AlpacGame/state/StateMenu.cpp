#include "StateMenu.h"


void StateMenu::goNext(StateMachine &stateMachine) {

    /// Assigning pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    window->setView(sf::View(window->getDefaultView()));
    menuGUI = &machine->configWindow.getMenuGUI();


    menuGUI->removeAllWidgets();
    windowWidth = tgui::bindWidth(machine->configWindow.getMenuGUI());
    windowHeight = tgui::bindHeight(machine->configWindow.getMenuGUI());
    picture = tgui::Picture::create("Resources/aluminium.jpg");
    picture->setSize(tgui::bindMax(800, windowWidth), tgui::bindMax(600, windowHeight));
    menuGUI->add(picture);

    // Creates a layout
    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    layout->setSize(windowWidth * 2 / 3, windowHeight / 2);
    layout->setPosition(windowWidth / 6, windowHeight / 6);


    // Creates a Play game button
    tgui::Button::Ptr button = theme->load("Button");
    button->setOpacity(50.f);
    button->setText("Play Game!");

    // Creates a Settings button
    tgui::Button::Ptr settingsButton = tgui::Button::copy(button);
    settingsButton->setText("Settings");

    // Creates a Quit button
    tgui::Button::Ptr quitButton = tgui::Button::copy(button);
    quitButton->setText("Quit");

    // Connects functions to the different buttons
    button->connect("pressed", [&] { machine->setCurrentState(StateMachine::stateID::SINGLEPLAYER); });
    settingsButton->connect("MousePressed", &StateMenu::changeOption, this);
    quitButton->connect("pressed", [&] { machine->setCurrentState(StateMachine::stateID::EXIT); });

    // Adds all the widgets and layouts to the MenuGUI
    layout->add(button);
    layout->addSpace();
    layout->add(settingsButton);
    layout->addSpace();
    layout->add(quitButton);
    layout->addSpace();
    menuGUI->add(layout);

    while (pollMenu()) {
        drawMenu();
    }
}

bool StateMenu::pollMenu() {
    sf::Event event;
    window->pollEvent(event);
    if (event.type == sf::Event::Closed) window->close();
    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::MENU;
}

void StateMenu::drawMenu() {
    window->clear(sf::Color::Black);
    menuGUI->draw();
    window->display();
}

void StateMenu::changeOption(){ machine->setCurrentState(StateMachine::stateID::OPTION); }
