
#include "StateGame.h"

void StateGame::goNext(StateMachine &stateMachine) {

    /// Assign pointers
    machine = &stateMachine;
    window = &machine->configWindow.getWindow();
    configGame = &machine->configGame;

    /// Clock used to throttle movement softly
    sf::Clock ticker;

    /// Initiating World
    world = new b2World(b2Vec2(0, 0));

    /// Instantiating initial entities
    planet = new Planet(world, configGame->planetRadius, window->getSize().x / 2, 600.f);
    farmer = new Farmer(world, configGame, 100, 100, 300, 200);
    entities.push_back(planet);
    entities.push_back(farmer);

    printf("Farmer Angle: %f", farmer->getBody()->GetAngle());

    /// Poll game
    while (pollGame()) {

        /// Catch deltaTime
        configGame->deltaTime = ticker.restart().asSeconds();

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
        world->Step(timeStep, velocityIterations, positionIterations);

        /// Gravity Calculation
        for (b2Body *bodyIter = world->GetBodyList(); bodyIter != nullptr; bodyIter = bodyIter->GetNext()) {
            float force = 10.0f;
            b2Vec2 delta = planet->getBody()->GetWorldCenter() - bodyIter->GetWorldCenter();
            delta.Normalize();
//            bodyIter->ApplyForce(force * delta, bodyIter->GetWorldPoint(b2Vec2(0, -5.f)), true);

            bodyIter->ApplyLinearImpulseToCenter(force * delta, true);

            // Adjust rotation every time
            float angle = atan2f(-delta.x, delta.y);
//            bodyIter->SetTransform(bodyIter->GetWorldCenter(), angle);

            float nextAngle = bodyIter->GetAngle() + bodyIter->GetAngularVelocity() / 60.0;
            float totalRotation = angle - nextAngle;
            while (totalRotation < -180 / DEGtoRAD) totalRotation += 360 / DEGtoRAD;
            while (totalRotation > 180 / DEGtoRAD) totalRotation -= 360 / DEGtoRAD;
            float desiredAngularVelocity = totalRotation * 60;
            float impulse = bodyIter->GetInertia() * desiredAngularVelocity;// disregard time factor
            bodyIter->ApplyAngularImpulse(impulse, true);
        }

        window->clear(sf::Color::Blue);

        for (Entity *e : entities) {
            e->switchAction();
            e->adjust();
            e->draw(*window);
        }

        window->display();

//        printf("Farmer Angle: %f", farmer->getBody()->GetAngle());

        // todo: Create a function for view?
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
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                machine->setCurrentState(StateMachine::stateID::MENU);
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

    switch (event.type) {
        case sf::Event::KeyPressed: {

            if(((Farmer*) farmer)->moveTimer.getElapsedTime().asSeconds() > ((Farmer*) farmer)->moveCoolDown){

                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) {
//                b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
//                farmer->getBody()->ApplyLinearImpulse((farmer->getBody()->GetWorldVector(b2Vec2(-4.0f, 0)) + delta),
//                                                      farmer->getBody()->GetWorldCenter(), true);

                    farmer->getBody()->ApplyLinearImpulseToCenter(10.f * farmer->getBody()->GetWorldVector(b2Vec2(-5.f, -10.f)),
                                                                  true);
                }
                if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) {

//                b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
//                farmer->getBody()->ApplyLinearImpulse((farmer->getBody()->GetWorldVector(b2Vec2(4.0f, 0)) + delta),
//                                                      farmer->getBody()->GetWorldCenter(), true);
//
                    farmer->getBody()->ApplyLinearImpulseToCenter(10.f * farmer->getBody()->GetWorldVector(b2Vec2(5.f, -10.f)),
                                                                  true);
                }

                if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::W) {

                    farmer->getBody()->ApplyLinearImpulseToCenter(10.f * farmer->getBody()->GetWorldVector(b2Vec2(15.f, -20.f)),
                                                                  true);
//                    b2Vec2 delta = planet->getBody()->GetWorldCenter() - farmer->getBody()->GetWorldCenter();
//                    farmer->getBody()->ApplyLinearImpulse(-10.0f * delta, farmer->getBody()->GetWorldCenter(), true);

                }

                ((Farmer*) farmer)->moveTimer.restart();
            }

        }

            break;

        default: {
            std::cout << "Random button" << std::endl;
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