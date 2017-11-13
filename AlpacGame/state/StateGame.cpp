
#include "StateGame.h"


void StateGame::goNext(StateMachine &stateMachine) {

    /// Assign pointers
    machine = &stateMachine;
    configGame = &machine->configGame;

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

    window->setMouseCursorVisible(false);

    /// Poll game
    while (pollGame()) {

        /// Save current mouse coordinates relatively to view
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
        configGame->mouseXpos = worldPos.x;
        configGame->mouseYpos = worldPos.y;

        double mouse = sf::Mouse::getPosition(*configGame->window).x;
        double center = configGame->window->getSize().x / 2;
        configGame->mouseInLeftSide = mouse < center;


        /// Save current input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            configGame->currentInput = sf::Keyboard::Right;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            configGame->currentInput = sf::Keyboard::Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            configGame->currentInput = sf::Keyboard::Up;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            configGame->currentInput = sf::Keyboard::E;
        } else {
            configGame->currentInput = sf::Keyboard::Unknown;
        }


        /// Box2D Physics Calculations
        // Iterating through all existing bodies
        for (b2Body *bodyIter = world->GetBodyList(); bodyIter != nullptr; bodyIter = bodyIter->GetNext()) {

            auto *entityInfo = (Entity *) bodyIter->GetUserData();
            if (!entityInfo->physicsSensitive)
                continue;

            // Calculate Radial Gravitation on all bodies
            float gravitationForce = 10.0f;
            float bodyMass = bodyIter->GetMass();
            b2Vec2 delta = planet->getBody()->GetWorldCenter() - bodyIter->GetWorldCenter();
            delta.Normalize();
            bodyIter->ApplyForceToCenter(gravitationForce * bodyMass * delta, true);

            // Calibrate rotation of entity according to planet's center by force
            float desiredAngle = atan2f(-delta.x, delta.y);
            auto nextAngle = static_cast<float>(bodyIter->GetAngle() + bodyIter->GetAngularVelocity() / 60.0);
            float totalRotation = desiredAngle - nextAngle;
            while (totalRotation < -180 / DEGtoRAD) totalRotation += 360 / DEGtoRAD;
            while (totalRotation > 180 / DEGtoRAD) totalRotation -= 360 / DEGtoRAD;
            float desiredAngularVelocity = totalRotation * 60;
            float impulse = bodyIter->GetInertia() * desiredAngularVelocity;// disregard time factor
            bodyIter->ApplyAngularImpulse(impulse, true);
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

        // todo fix aim
        sf::CircleShape mouseAim;
        mouseAim.setRadius(5);
        mouseAim.setPosition(configGame->mouseXpos, configGame->mouseYpos);
        mouseAim.setFillColor(sf::Color::Red);
        mouseAim.setOutlineColor(sf::Color::Black);
        mouseAim.setOutlineThickness(5);
        window->draw(mouseAim);

        /// Update View
        window->display();


        // Finding angle of farmer
        b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
        delta.Normalize();
        float angle = atan2f(-delta.x, delta.y);

        // Coordinates of the surface in which the farmer is standing on
        float viewX = configGame->calcX(angle * DEGtoRAD);
        float viewY = configGame->calcY(angle * DEGtoRAD);

        view.setCenter(viewX, viewY);

        view.setRotation(angle * DEGtoRAD);

        window->setView(view);

    }
}

bool StateGame::pollGame() {
    sf::Event event{};
    while (window->pollEvent(event)) {
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
//                    machine->setCurrentState(StateMachine::stateID::PAUSE);
                    return false;
                } else {
                    keyPressedHandler(event);
                }
            }
            case sf::Event::MouseButtonPressed: {
                mousePressedHandler(event);
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
        case sf::Keyboard::I: {
            configGame->showLabels = !configGame->showLabels;
            break;
        }
        case sf::Keyboard::R: {
            configGame->newGame = true;

            break;
        }
        case sf::Keyboard::P: {
            printf("X pos: %f\n", configGame->mouseXpos);
            printf("Y pos: %f\n\n", configGame->mouseYpos);
            break;
        }
        case sf::Keyboard::Num1: {
            entities->emplace_back(new Alpaca(configGame, 40, 100, 100, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::Num2: {
            entities->emplace_back(new Wolf(configGame, 40, 90, 90, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::Num3: {
            entities->emplace_back(
                    new Shotgun(configGame, 80, 20, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::Z: {
            if (zoomed) {
                zoomed = false;
                view = sf::View(window->getDefaultView());
                view.zoom(viewNonZoomed);
            } else {
                zoomed = true;
                view = sf::View(window->getDefaultView());
                view.zoom(viewZoomed);
            }
            break;
        }
        default: {
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