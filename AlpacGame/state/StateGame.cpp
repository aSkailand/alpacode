
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
        view = sf::View(window->getDefaultView());

        view.zoom(1.5f);

        // Finding able of farmer
        b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
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

        case sf::Keyboard::Unknown:break;
        case sf::Keyboard::A:break;
        case sf::Keyboard::B:break;
        case sf::Keyboard::C:break;
        case sf::Keyboard::D:break;
        case sf::Keyboard::E:break;
        case sf::Keyboard::F:break;
        case sf::Keyboard::G:break;
        case sf::Keyboard::H:break;
        case sf::Keyboard::I: configGame->showLabels = !configGame->showLabels; break;
        case sf::Keyboard::J:break;
        case sf::Keyboard::K:break;
        case sf::Keyboard::L:break;
        case sf::Keyboard::M:break;
        case sf::Keyboard::N:break;
        case sf::Keyboard::O:break;
        case sf::Keyboard::P:break;
        case sf::Keyboard::Q:break;
        case sf::Keyboard::R:break;
        case sf::Keyboard::S:break;
        case sf::Keyboard::T:break;
        case sf::Keyboard::U:break;
        case sf::Keyboard::V:break;
        case sf::Keyboard::W:break;
        case sf::Keyboard::X:break;
        case sf::Keyboard::Y:break;
        case sf::Keyboard::Z:break;
        case sf::Keyboard::Num0:break;
        case sf::Keyboard::Num1:break;
        case sf::Keyboard::Num2:break;
        case sf::Keyboard::Num3:break;
        case sf::Keyboard::Num4:break;
        case sf::Keyboard::Num5:break;
        case sf::Keyboard::Num6:break;
        case sf::Keyboard::Num7:break;
        case sf::Keyboard::Num8:break;
        case sf::Keyboard::Num9:break;
        case sf::Keyboard::Escape:break;
        case sf::Keyboard::LControl:break;
        case sf::Keyboard::LShift:break;
        case sf::Keyboard::LAlt:break;
        case sf::Keyboard::LSystem:break;
        case sf::Keyboard::RControl:break;
        case sf::Keyboard::RShift:break;
        case sf::Keyboard::RAlt:break;
        case sf::Keyboard::RSystem:break;
        case sf::Keyboard::Menu:break;
        case sf::Keyboard::LBracket:break;
        case sf::Keyboard::RBracket:break;
        case sf::Keyboard::SemiColon:break;
        case sf::Keyboard::Comma:break;
        case sf::Keyboard::Period:break;
        case sf::Keyboard::Quote:break;
        case sf::Keyboard::Slash:break;
        case sf::Keyboard::BackSlash:break;
        case sf::Keyboard::Tilde:break;
        case sf::Keyboard::Equal:break;
        case sf::Keyboard::Dash:break;
        case sf::Keyboard::Space:break;
        case sf::Keyboard::Return:break;
        case sf::Keyboard::BackSpace:break;
        case sf::Keyboard::Tab:break;
        case sf::Keyboard::PageUp:break;
        case sf::Keyboard::PageDown:break;
        case sf::Keyboard::End:break;
        case sf::Keyboard::Home:break;
        case sf::Keyboard::Insert:break;
        case sf::Keyboard::Delete:break;
        case sf::Keyboard::Add:break;
        case sf::Keyboard::Subtract:break;
        case sf::Keyboard::Multiply:break;
        case sf::Keyboard::Divide:break;
        case sf::Keyboard::Left:break;
        case sf::Keyboard::Right:break;
        case sf::Keyboard::Up:break;
        case sf::Keyboard::Down:break;
        case sf::Keyboard::Numpad0:break;
        case sf::Keyboard::Numpad1:break;
        case sf::Keyboard::Numpad2:break;
        case sf::Keyboard::Numpad3:break;
        case sf::Keyboard::Numpad4:break;
        case sf::Keyboard::Numpad5:break;
        case sf::Keyboard::Numpad6:break;
        case sf::Keyboard::Numpad7:break;
        case sf::Keyboard::Numpad8:break;
        case sf::Keyboard::Numpad9:break;
        case sf::Keyboard::F1:break;
        case sf::Keyboard::F2:break;
        case sf::Keyboard::F3:break;
        case sf::Keyboard::F4:break;
        case sf::Keyboard::F5:break;
        case sf::Keyboard::F6:break;
        case sf::Keyboard::F7:break;
        case sf::Keyboard::F8:break;
        case sf::Keyboard::F9:break;
        case sf::Keyboard::F10:break;
        case sf::Keyboard::F11:break;
        case sf::Keyboard::F12:break;
        case sf::Keyboard::F13:break;
        case sf::Keyboard::F14:break;
        case sf::Keyboard::F15:break;
        case sf::Keyboard::Pause:break;
        case sf::Keyboard::KeyCount:break;
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