
#include "StateGame.h"


void StateGame::goNext(StateMachine &stateMachine) {
    /// Assign pointers
    machine = &stateMachine;
    configGame = &machine->configGame;
    configSound = &machine->configSound;

    menuGUI = machine->configWindow.getMenuGUI();
    menuGUI->removeAllWidgets();
    // Adds the score label to the window
    menuGUI->add(machine->configMenu->getScoreLabel());
    //temp int score
    int score = 0;

    /// Reset Game
    if (configGame->newGame) {

        configGame->reset();

        /// Assign pointers
        window = &machine->configWindow.getWindow();
        world = configGame->world;
        entities = configGame->entities;
        planet = configGame->planet;
        farmer = dynamic_cast<Farmer *> (configGame->farmer);

        configGame->newGame = false;
    }

    /// View
    view = sf::View(window->getDefaultView());
    view.zoom(viewNonZoomed);

    // todo fix aim
    sf::CircleShape mouseAim;
    mouseAim.setRadius(5);
    mouseAim.setFillColor(sf::Color::Red);
    mouseAim.setOutlineColor(sf::Color::Black);
    mouseAim.setOutlineThickness(5);

    /// Play music
    if (!machine->configSound.isGameMusicPlaying) {
        machine->configSound.playGameMusic(true);
    }

    /// Poll game
    while (pollGame()) {

        /// Temp score setter
        machine->configMenu->setScore(std::to_string(score++));

        /// Save current mouse coordinates relatively to view
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
        configGame->mouseXpos = worldPos.x;
        configGame->mouseYpos = worldPos.y;

        double mouse = sf::Mouse::getPosition(*configGame->window).x;
        double center = configGame->window->getSize().x / 2;
        configGame->mouseInLeftSide = mouse < center;


        /// Save current input
        configGame->currentCommand = ConfigGame::ControlName::NOTHING;
        for (auto &MapControlKey : configGame->MapControlKeys) {
            if(sf::Keyboard::isKeyPressed(MapControlKey.second)){
                configGame->currentCommand = MapControlKey.first;
                break;
            }
        }

        /*
        if (sf::Keyboard::isKeyPressed(configGame->MapControlKeys[ConfigGame::ControlName::JUMP])) {
            configGame->currentInput = configGame->MapControlKeys[ConfigGame::ControlName::JUMP];
        } else if (sf::Keyboard::isKeyPressed(configGame->MapControlKeys[ConfigGame::ControlName::LEFT])) {
            configGame->currentInput = configGame->MapControlKeys[ConfigGame::ControlName::LEFT];
        } else if (sf::Keyboard::isKeyPressed(configGame->MapControlKeys[ConfigGame::ControlName::RIGHT])) {
            configGame->currentInput = configGame->MapControlKeys[ConfigGame::ControlName::RIGHT];
        } else if (sf::Keyboard::isKeyPressed(configGame->MapControlKeys[ConfigGame::ControlName::GRAPS])) {
            configGame->currentInput = configGame->MapControlKeys[ConfigGame::ControlName::GRAPS];
        } else {
            configGame->currentInput = configGame->MapControlKeys[ConfigGame::ControlName::NOTHING];
        }
        */

        /// Box2D Physics Calculations
        // Iterating through all existing bodies
        for (b2Body *bodyIter = world->GetBodyList(); bodyIter != nullptr; bodyIter = bodyIter->GetNext()) {

            if (!bodyIter->IsAwake())
                continue;

            // Calculate Radial Gravitation on all bodies
            float gravitationForce = 10.0f;
            float bodyMass = bodyIter->GetMass();
            b2Vec2 delta = planet->getBody()->GetWorldCenter() - bodyIter->GetWorldCenter();
            delta.Normalize();
            bodyIter->ApplyForceToCenter(gravitationForce * bodyMass * delta, false);

            // Calibrate rotation of entity according to planet's center by force
            float desiredAngle = atan2f(-delta.x, delta.y);
            auto nextAngle = static_cast<float>(bodyIter->GetAngle() + bodyIter->GetAngularVelocity() / 60.0);
            float totalRotation = desiredAngle - nextAngle;
            while (totalRotation < -180 / DEGtoRAD) totalRotation += 360 / DEGtoRAD;
            while (totalRotation > 180 / DEGtoRAD) totalRotation -= 360 / DEGtoRAD;
            float desiredAngularVelocity = totalRotation * 60;
            float impulse = bodyIter->GetInertia() * desiredAngularVelocity;// disregard time factor
            bodyIter->ApplyAngularImpulse(impulse, false);
        }

        /// Box2D World Step
        world->Step(timeStep, velocityIterations, positionIterations);

        /// Render Phase
        window->clear(sf::Color::Blue);


        /// Delete Dead Entities
        for (auto entityIter = entities->begin(); entityIter != entities->end(); ++entityIter) {
            if ((*entityIter)->deadCheck()) {

                world->DestroyBody((*entityIter)->getBody());

                delete (*entityIter);

                entityIter = entities->erase(entityIter);

                if (entityIter == entities->end()) {
                    break;
                }

            }
        }

        /// Activate all warm entities
        for (Entity *e : *entities) {

            // Check if current entity is an warm entity
            auto warm_e = dynamic_cast<EntityWarm *> (e);
            if (warm_e != nullptr) {
                warm_e->switchAction();
                warm_e->performAction();

            }

            // Adjust SFML shape to Box2D body's position and rotation, then draw it.
            e->render(window);
        }


        mouseAim.setPosition(configGame->mouseXpos, configGame->mouseYpos);
        window->draw(mouseAim);

        // Finding angle of farmer
        b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
        delta.Normalize();
        float angle = atan2f(-delta.x, delta.y);

        // Coordinates of the surface in which the farmer is standing on
        float viewX = configGame->calcX(angle * DEGtoRAD);
        float viewY = configGame->calcY(angle * DEGtoRAD);

        view.setCenter(viewX, viewY);

        if (testDefeat) {
            menuGUI->draw();
        }


        view.setRotation(angle * DEGtoRAD);

        window->setView(view);

        /// Update View
        menuGUI->draw();
        window->display();

    }
}

bool StateGame::pollGame() {
    sf::Event event;
    while (window->pollEvent(event)) {
        menuGUI->handleEvent(event);
        switch (event.type) {
            case sf::Event::Closed: {
                machine->setCurrentState(StateMachine::stateID::EXIT);
                return false;
            }
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::Escape) {
                    machine->setCurrentState(StateMachine::stateID::MENU);
                    return false;
                } else if (event.key.code == sf::Keyboard::Space) {
                    // todo: Add actual pause.
                    //machine->setCurrentState(StateMachine::stateID::PAUSE);
                    return false;
                } else {
                    keyPressedHandler(event);
                }
                break;
            }
            case sf::Event::MouseButtonPressed: {
                mousePressedHandler(event);
                break;
            }
            default: {
                return true;
            }
        }
    }

    return !configGame->newGame;

}


void StateGame::keyPressedHandler(sf::Event event) {

    switch (event.key.code) {
        case sf::Keyboard::F9: {
            configGame->showLabels = !configGame->showLabels;
            break;
        }
        case sf::Keyboard::F8: {
            configGame->newGame = true;

            break;
        }
        case sf::Keyboard::P: {
            printf("X pos: %f\n", configGame->mouseXpos);
            printf("Y pos: %f\n\n", configGame->mouseYpos);
            menuGUI->removeAllWidgets();
            menuGUI->add(machine->configMenu->mapLayouts[ConfigMenu::layouts::DEFEAT]);
            testDefeat = !testDefeat;
            break;
        }
        case sf::Keyboard::F1: {
            entities->emplace_back(new Alpaca(configGame, 40, 100, 100, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::F2: {
            entities->emplace_back(new Wolf(configGame, 40, 150, 100, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::F3: {
            entities->emplace_back(
                    new Shotgun(configGame, configSound, 100, 25, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        default: {
            if(event.key.code == configGame->MapControlKeys[ConfigGame::ControlName::ZOOM]){
                if (zoomed) {
                    zoomed = false;
                    view = sf::View(window->getDefaultView());
                    view.zoom(viewNonZoomed);
                } else {
                    zoomed = true;
                    view = sf::View(window->getDefaultView());
                    view.zoom(viewZoomed);
                }}
            break;
        }
    }
}


void StateGame::mousePressedHandler(sf::Event event) {

    switch (event.mouseButton.button) {
        case sf::Mouse::Left: {
            if (dynamic_cast<Usable *>(farmer->holdingEntity)) {
                dynamic_cast<Usable *>(farmer->holdingEntity)->use();
            }
            break;
        }
        case sf::Mouse::Right: {
            break;
        }
        default: {
            break;
        }
    }

}

