
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
        dayCycle = configGame->dayCycle;
        dayCycle->initiateClock();

        configGame->newGame = false;

        /// View
        view = sf::View(window->getDefaultView());
        view.zoom(viewNonZoomed);

        window->setMouseCursorVisible(false);
    }

    // todo fix aim
    sf::CircleShape mouseAim;
    mouseAim.setRadius(5);
    mouseAim.setFillColor(sf::Color::Red);
    mouseAim.setOutlineColor(sf::Color::Black);
    mouseAim.setOutlineThickness(5);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            configGame->currentInput = sf::Keyboard::W;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            configGame->currentInput = sf::Keyboard::A;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            configGame->currentInput = sf::Keyboard::D;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            configGame->currentInput = sf::Keyboard::E;
        } else {
            configGame->currentInput = sf::Keyboard::Unknown;
        }

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

        dayCycle->proceed();

        /// Render Phase
        window->clear(sf::Color::Blue);

        /// Draw Background;
        for( Scenery *s : *configGame->sceneries){
            s->render(window);
        }

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

        /// Spawn Alpacas
        while (!configGame->queue.empty()){
            b2Vec2 babySpawnPos = configGame->queue.front();
            configGame->queue.pop();
            auto* babyAlpaca = new Alpaca(configGame, false, babySpawnPos.x * SCALE, babySpawnPos.y * SCALE);
            b2Vec2 delta = planet->getBody()->GetWorldCenter() - babyAlpaca->getBody()->GetWorldCenter();
            delta.Normalize();
            float mass = babyAlpaca->getBody()->GetMass();
            babyAlpaca->getBody()->ApplyLinearImpulseToCenter(mass * 2.f * -delta, true);
            entities->push_back(babyAlpaca);
        }

        /// Spawn Wolves
        if(configGame->spawnWolves){
            if(configGame->wolfSpawnTimer.getElapsedTime().asSeconds() > 1.f){

                entities->push_back(new Wolf(configGame, configGame->wolfDenPos.x, configGame->wolfDenPos.y));

                configGame->currentWolves += 1.f;
                configGame->wolfSpawnTimer.reset(true);

                if(configGame->currentWolves >= configGame->maxWolves){

                    configGame->spawnWolves = false;
                    configGame->currentWolves = 0.f;
                }
            }
        }

        // todo: Make the defeat check here
        unsigned int numAliveAlpacas = 0;

        /// Activate all warm entities
        for (Entity *e : *entities) {
            // Check if current entity is an warm entity
            auto warm_e = dynamic_cast<EntityWarm *> (e);
            if (warm_e != nullptr) {

                // Update Warm Entity Action
                warm_e->switchAction();
                warm_e->performAction();

                // Check if mouse pointer is hovering over the entity
                warm_e->currentlyMousedOver = warm_e->get_sf_ShapeEntity()->getGlobalBounds().contains(
                        window->mapPixelToCoords(sf::Mouse::getPosition(*window)));

                if(warm_e->getEntity_ID() == Entity::ID::ALPACA && warm_e->currentHealth == EntityWarm::Health::ALIVE){
                    numAliveAlpacas++;
                }
            }
        }

        // todo: Do a defeat check here

        /// Update all cold entities
        for (Entity *e : *entities) {
            auto cold_e = dynamic_cast<EntityCold *> (e);
            if (cold_e != nullptr) {
                cold_e->update();
            }
        }





        /// Draw aim
        mouseAim.setPosition(configGame->mouseXpos, configGame->mouseYpos);
        window->draw(mouseAim);

        // Adjust SFML shape to Box2D body's position and rotation, then draw it.
        for (Entity *e : *entities) {
            e->render(window);
        }

        configGame->mouseArrow.setPosition(configGame->mouseXpos, configGame->mouseYpos);
        window->draw(configGame->mouseArrow);

        /// Check if game is over
        if (farmer != nullptr && farmer->currentHealth == EntityWarm::Health::ALIVE) {

            // Finding angle of farmer
            b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
            delta.Normalize();
            float angle = atan2f(-delta.x, delta.y);

            // Coordinates of the surface in which the farmer is standing on
            float viewX = configGame->calcX(angle * DEGtoRAD);
            float viewY = configGame->calcY(angle * DEGtoRAD);

            // Set view accordingly to farmer
            view.setCenter(viewX, viewY);
            view.setRotation(angle * DEGtoRAD);

            // Activate view
            window->setView(view);
        }

        /// Update View
        window->display();

    }
}

bool StateGame::pollGame() {
    sf::Event event;
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
                } else if (event.key.code == sf::Keyboard::P) {
                    machine->setCurrentState(StateMachine::stateID::PAUSE);
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
        case sf::Keyboard::I: {
            configGame->showDebugMode = !configGame->showDebugMode;
            std::cout << "Number of alive alpacas: " << configGame->numOfAliveAlpacas << std::endl;
            break;
        }
        case sf::Keyboard::R: {
            if (!configGame->isPaused) {
                configGame->newGame = true;
            }
            break;
        }

        case sf::Keyboard::Num1: {
            entities->emplace_back(new Alpaca(configGame, true, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::Num2: {
            entities->emplace_back(new Wolf(configGame, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::Num3: {
            entities->emplace_back(new Shotgun(configGame, 100, 25, configGame->mouseXpos, configGame->mouseYpos));
            break;
        }
        case sf::Keyboard::Num4: {
            entities->emplace_back(
                    new Trap(configGame, 150, 75, configGame->mouseXpos, configGame->mouseYpos));
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
            if (!configGame->isPaused && dynamic_cast<Usable *>(farmer->holdingEntity)) {
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

