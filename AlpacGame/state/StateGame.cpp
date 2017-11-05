
#include "StateGame.h"

void StateGame::goNext(StateMachine &stateMachine) {

    /// Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    configGame = &machine->configGame;

    /// Initiating World (With no innate gravitation)
    world = new b2World(b2Vec2(0, 0));
    world->SetContactListener(new CollisionListener());

    /// Instantiating initial entities
    planet = new Planet(world, configGame->planetRadius, 0, 0);
    farmer = new Farmer(world, configGame, 100, 100, 0, -200);
    entities.push_back(planet);
    entities.push_back(farmer);

    /// View
    float f_x, f_y;
    view = sf::View(window->getDefaultView());
    view.zoom(1.5f);

    /// Poll game
    while (pollGame()) {

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
            auto warm_e = dynamic_cast<EntityWarm *> (e);
            if (warm_e != nullptr) {
                warm_e->switchAction();
                warm_e->performAction();
            }

            // Adjust SFML shape to Box2D body's position and rotation, then draw it.
            e->render(window);
        }

        /// Update View
        window->display();


        // Finding angle of farmer
        b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
        delta.Normalize();
        float angle = atan2f(-delta.x, delta.y);

        // Coordinates of the surface in which the farmer is standing on
        f_y = (configGame->planetRadius * sin(angle - b2_pi/2.0f) );
        f_x = (configGame->planetRadius * cos(angle - b2_pi/2.0f) );

        view.setCenter(f_x , f_y);

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
    return true;
}


void StateGame::keyPressedHandler(sf::Event event) {
    switch(event.key.code){
        case sf::Keyboard::I: {
            configGame->showLabels = !configGame->showLabels;
            break;
        }
        default:{
            break;
        }
    }
}


void StateGame::mousePressedHandler(sf::Event event) {

    // Save Mouse Coordinates
    int mouseX = sf::Mouse::getPosition(*window).x;
    int mouseY = sf::Mouse::getPosition(*window).y;


    switch (event.mouseButton.button) {
        case sf::Mouse::Left: {
            entities.push_back(new Alpaca(world, configGame, 100, mouseX, mouseY));
            break;
        }
        case sf::Mouse::Right: {
            entities.push_back(new Wolf(world, configGame, 100, mouseX, mouseY));
            break;
        }
        default: {
            break;
        }
    }

}