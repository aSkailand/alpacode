
#include "StatePause.h"
#include "StateMachine.h"

void StatePause::goNext(StateMachine &stateMachine) {

    /// Assigning Pointers
    this->stateMachine = &stateMachine;
    configGame = &stateMachine.configGame;
    window = &stateMachine.configWindow.getWindow();
    menuGUI = stateMachine.configWindow.getMenuGUI();

    /// Pausing

    // Set pause bool
    stateMachine.configGame.isPaused = true;

    // Set the mouse cursor visible
    stateMachine.configWindow.getWindow().setMouseCursorVisible(true);

    // View
    sf::View view = sf::View(window->getDefaultView());

    // Create Pause Filter
    sf::RectangleShape pauseFilter;
    pauseFilter.setFillColor(sf::Color(100, 100, 100, 150));
    pauseFilter.setSize(sf::Vector2f(6000, 6000));
    pauseFilter.setOrigin(sf::Vector2f(3000, 3000));
    pauseFilter.setPosition(window->mapPixelToCoords(sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2)));

    // Pause DayCycle
    configGame->dayCycle->pause();

    // Pause entities
    for (auto &e : *configGame->entities) {
        e->pause();
    }

    // Add Pause Menu
    menuGUI->removeAllWidgets();
    menuGUI->add(stateMachine.configMenu->mapLayouts[ConfigMenu::layouts::PAUSE]);

    /// Poll Paused Game
    while (pollGame()) {

        /// Render Phase
        window->clear(sf::Color::Blue);

        // Draw sceneries
        for (Scenery *s : *configGame->sceneries) {
            s->render(window);
        }

        // Draw entities
        for (Entity *e : *configGame->entities) {
            e->render(configGame->window);
        }

        // Draw Filter
        window->draw(pauseFilter);

        // Draw screen
        menuGUI->draw();
        window->display();
    }
}

bool StatePause::pollGame() {
    sf::Event event{};
    while (window->pollEvent(event)) {
        menuGUI->handleEvent(event);
        switch (event.type) {
            case sf::Event::Closed: {
                stateMachine->setCurrentState(StateMachine::stateID::EXIT);
                break;
            }
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::P) {
                    stateMachine->setCurrentState(StateMachine::stateID::SINGLEPLAYER);
                    break;
                }
            }
            default:
                break;
        }
    }

    // Check what state is current
    if (stateMachine->getCurrentState() != StateMachine::stateID::PAUSE) {

        // Turn off logic pause
        stateMachine->configGame.isPaused = stateMachine->getCurrentState() == StateMachine::stateID::OPTION;

        // Set the mouse cursor invisible
        if (stateMachine->getCurrentState() == StateMachine::stateID::SINGLEPLAYER)
            stateMachine->configWindow.getWindow().setMouseCursorVisible(false);

        // Resume DayCycle
        configGame->dayCycle->resume();

        // Resume entities
        for (auto &e : *configGame->entities) {
            e->resume();
        }

        // End StatePause if new state is queued
        return false;

    } else {

        // Resume StatePause if no new state is queued
        return true;

    }
}

