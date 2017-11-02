#include "StateMenu.h"


void StateMenu::goNext(StateMachine &stateMachine) {

    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    window->setView(sf::View(window->getDefaultView()));
    menuGUI = &machine->configWindow.getMenuGUI();
    menuGUI->removeAllWidgets();


    auto windowWidth = tgui::bindWidth(machine->configWindow.getMenuGUI());
    auto windowHeight = tgui::bindHeight(machine->configWindow.getMenuGUI());
    auto picture = tgui::Picture::create("aluminium.jpg");
    picture->setSize(tgui::bindMax(800,windowWidth), tgui::bindMax(600,windowHeight));
    menuGUI->add(picture);

    //TODO: Make theme and layout pointers?
    // Creates a theme
    tgui::Theme::Ptr theme = tgui::Theme::create("C:/dev/libs/TGUI/include/TGUI/widgets/BabyBlue.txt");

    // Creates a layout
    tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
    layout->setSize(windowWidth * 2/3, windowHeight / 2);
    layout->setPosition(windowWidth / 6, windowHeight / 6);


    // Creates a button
    tgui::Button::Ptr button = theme->load("Button");
    button->setOpacity(50.f);
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
