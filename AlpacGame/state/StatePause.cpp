
#include "StatePause.h"
#include "StateMachine.h"

void StatePause::goNext(StateMachine &stateMachine) {

    /// Assigning Pointers
    this->stateMachine = &stateMachine;
    configGame = &stateMachine.configGame;
    window = &stateMachine.configWindow.getWindow();
    menuGUI = stateMachine.configWindow.getMenuGUI();

    /// Pausing
    configGame->isPaused = true;
    for (auto &e : *configGame->entities) {
        e->pause();
    }

    /// Set the mouse coursor visible
    stateMachine.configWindow.getWindow().setMouseCursorVisible(true);

    /// View
    sf::View view = sf::View(window->getDefaultView());

    /// Create Pause Filter
    sf::RectangleShape pauseFilter;
    pauseFilter.setFillColor(sf::Color(100, 100, 100, 150));
    pauseFilter.setSize(sf::Vector2f(6000, 6000));
    pauseFilter.setOrigin(sf::Vector2f(3000, 3000));
    pauseFilter.setPosition(window->mapPixelToCoords(sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2)));

    menuGUI->removeAllWidgets();
    menuGUI->add(stateMachine.configMenu->mapLayouts[ConfigMenu::layouts::PAUSE]);

    while(pollGame()){

        /// Render Phase
        window->clear(sf::Color::Blue);

        for( Scenery *s : *configGame->sceneries){
            s->render(window);
        }

        /// Adjust SFML shape to Box2D body's position and rotation, then draw it.
        for (Entity *e : *configGame->entities) {
            e->render(configGame->window);
        }

        /// Draw Filter
        window->draw(pauseFilter);

        /// Save current mouse coordinates relatively to view
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
        configGame->mouseXpos = worldPos.x;
        configGame->mouseYpos = worldPos.y;

        double mouse = sf::Mouse::getPosition(*configGame->window).x;
        double center = configGame->window->getSize().x / 2;
        configGame->mouseInLeftSide = mouse < center;

        configGame->mouseArrow.setPosition(configGame->mouseXpos, configGame->mouseYpos);
        configGame->window->draw(configGame->mouseArrow);

        /// Update View
        menuGUI->draw();
        window->display();
    }
}

bool StatePause::pollGame() {
    sf::Event event{};
    while(window->pollEvent(event)){
        menuGUI->handleEvent(event);
        switch(event.type){
            case sf::Event::Closed:{
                stateMachine->setCurrentState(StateMachine::stateID::EXIT);
                return false;
            }
            case sf::Event::KeyPressed:{
                if(event.key.code == sf::Keyboard::P){
                    configGame->isPaused = false;
                    for (auto &e : *configGame->entities) {
                        e->resume();
                    }
                    stateMachine->setCurrentState(StateMachine::stateID::SINGLEPLAYER);
                    return false;
                }
            }
            default:
                break;
        }
    }
    return stateMachine->configMenu->pauseBool;
}

