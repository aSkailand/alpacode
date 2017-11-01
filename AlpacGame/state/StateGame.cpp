
#include "StateGame.h"

void StateGame::goNext(StateMachine &stateMachine) {

    /// Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    configGame = &machine->configGame;

    /// Initiating World (With no innate gravitation)
    world = new b2World(b2Vec2(0, 0));

    /// Instantiating initial entities
    planet = new Planet(world, configGame->planetRadius, window->getSize().x / 2, 600.f);
    farmer = new Farmer(world, configGame, 100, 100, 300, 200);
    entities.push_back(planet);
    entities.push_back(farmer);

    /// Poll game
    while (pollGame()) {

        /// Save current input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            configGame->currentInput = sf::Keyboard::Right;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            configGame->currentInput = sf::Keyboard::Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            configGame->currentInput = sf::Keyboard::Up;
        } else {
            configGame->currentInput = sf::Keyboard::Unknown;
        }

        /// Box2D Physics Calculations
        // Iterating through all existing bodies
        for (b2Body *bodyIter = world->GetBodyList(); bodyIter != nullptr; bodyIter = bodyIter->GetNext()) {

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

        for (Entity *e : entities) {

            // Check if current entity is an warm entity
            if(dynamic_cast<EntityWarm*> (e) != nullptr){
                dynamic_cast<EntityWarm*> (e)->switchAction();
            }

            // Adjust SFML shape to Box2D body's position and rotation
            e->adjust();

            // Draw SFML shape on game window
            e->draw(*window);
        }

        window->display();

        /// Update View
//         Update View
        view = sf::View(window->getDefaultView());
        view.zoom(3.f);
//        view.zoom(viewZoom);
        view.setCenter(planet->x, planet->y);
//        view.setCenter(farmer->x, farmer->y);
//        view.setRotation(farmer->getBody()->GetAngle() * DEGtoRAD);
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
    return true;
}


void StateGame::keyPressedHandler(sf::Event event) {
}


void StateGame::mousePressedHandler(sf::Event event) {

    // Save Mouse Coordinates
    int mouseX = sf::Mouse::getPosition(*window).x;
    int mouseY = sf::Mouse::getPosition(*window).y;


    switch (event.mouseButton.button) {
        case sf::Mouse::Left: {
            entities.push_back(new Alpaca(world, planet->getBody(), 100, 100, mouseX, mouseY));
            break;
        }
        case sf::Mouse::Right: {
            entities.push_back(new Wolf(world, planet->getBody(), 100, 100, mouseX, mouseY));
            break;
        }
        default: {
            break;
        }
    }

}