#include "StateOption.h"

void StateOption::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    menuGUI = &machine->configWindow.getMenuGUI();
    menuGUI->removeAllWidgets();
    // TODO: Make theme and layout pointers?
    // Creates a theme
    tgui::Theme::Ptr theme = tgui::Theme::create("C:/dev/libs/TGUI/include/TGUI/widgets/Black.txt");

    // Creates a layout
    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    layout->setSize(400, 400);
    layout->setPosition(machine->configWindow.getScreenWidth() / 4, machine->configWindow.getScreenHeight() / 4);


    tgui::ListBox::Ptr boxx = theme->load("ListBox");
    boxx->addItem("Skrra");
    boxx->addItem("Boom");

    layout->add(boxx);

    // Creates a panel
    tgui::Scrollbar::Ptr panel = theme->load("Scrollbar");
    layout->add(panel);

    // Creates a button
    tgui::Button::Ptr optButton = theme->load("Button");
    optButton->setText("Low resouluton (1280x720)");

    tgui::Button::Ptr optButton1 = tgui::Button::copy(optButton);
    optButton1->setText("Medium resoulution (1920x1080)");

    tgui::Button::Ptr optButton2 = tgui::Button::copy(optButton);
    optButton2->setText("Back");


    optButton->connect("pressed", StateOption::lowRes, this);
    optButton1->connect("pressed", StateOption::highRes, this);
    optButton2->connect("pressed", [this]{machine->setCurrentState(StateMachine::stateID::MENU);});


    layout->add(optButton);
    layout->addSpace();
    layout->add(optButton1);
    layout->addSpace();
    layout->add(optButton2);
    menuGUI->add(layout);

    while (PollOption()){
        drawOption();
    }
}

bool StateOption::PollOption() {
    sf::Event event;
    window->pollEvent(event);

    if(event.type == sf::Event::Closed) window->close();
    menuGUI->handleEvent(event);
    return machine->getCurrentState() == StateMachine::stateID::OPTION;
}

void StateOption::drawOption() {
    window->clear(sf::Color::Black);
    menuGUI->draw();
    window->display();
}

void StateOption::lowRes() {
    machine->configWindow.setWindowResolution(1280, 720);
    machine->configWindow.setScreenWidth(1280);
    machine->configWindow.setScreenHeight(720);
}

void StateOption::highRes() {
    machine->configWindow.setWindowResolution(1920, 1080);
    machine->configWindow.setScreenWidth(1920);
    machine->configWindow.setScreenHeight(1080);
}