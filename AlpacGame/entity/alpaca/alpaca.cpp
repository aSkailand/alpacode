#include "Alpaca.h"

//Alpaca::Alpaca(StateMachine &stateMachine, float initAngle) : id(nextId++) {
//
//    // Assigning pointers
//    configGame = &stateMachine.configGame;
//    window = &stateMachine.configWindow.getWindow();
//
//    // Load textures
//    loadTextures();
//
//    // Assigning default states
//    currentAction = Action::IDLE;
//    currentDirection = Direction::RIGHT;
//
//    // Generate random number generator
//    long long int seed = std::chrono::system_clock::now().time_since_epoch().count() + id;
//    generator = std::default_random_engine(seed);
//
//    // Define the alpaca
//    alpacashape = sf::RectangleShape(sf::Vector2f(size, size));
//    alpacashape.setTexture(&alpacaTexture);
//    alpacashape.setOrigin(alpacashape.getSize().x / 2, alpacashape.getSize().y);
//    alpacashape.setOutlineThickness(1);
//
//    // Assigning initial position
//    angle = initAngle;
//}

//void Alpaca::draw() {
//
//    /* The reason why setPosition and setRotation is here and not in control() is
//     * because when adding movement to alpacas, the x and y change one more time (First
//     * time being the reposition because of the planet's rotation).
//     * Therefore we don't want setPosition and setRotation in control(), where
//     * this movement will be handled. */
//
//    // Place the alpaca accordingly
//    alpacashape.setRotation(angle);
//    alpacashape.setPosition(x, y);
//
//    // Draw the square
//    window->draw(alpacashape);
//void Alpaca::control() {
//
//     Check if it is time for randomizing the alpaca's current state
//    if ((int) clock.getElapsedTime().asSeconds() >= tickSecond) {
//
//        currentAction = (Action) randomNumberGenerator(0, 1);
//
//        switch (currentAction) {
//            case Alpaca::Action::IDLE:{
//                std::cout << "Alpaca " << id << " is now IDLE." << std::endl;
//                break;
//            }
//            case Alpaca::Action::WALKING: {
//                currentDirection = (Direction) randomNumberGenerator(0, 1);
//                switch (currentDirection) {
//                    case Alpaca::Direction::LEFT: {
//                        std::cout << "Alpaca " << id << " is now WALKING LEFT" << std::endl;
//                        alpacashape.setScale({-1.f, 1.f});
//                        break;
//                    }
//                    case Alpaca::Direction::RIGHT: {
//                        std::cout << "Alpaca " << id << " is now WALKING RIGHT" << std::endl;
//                        alpacashape.setScale({1.f, 1.f});
//                        break;
//                    }
//                }
//                break;
//            }
//        }
//
//        clock.restart();
//
//    }
//
//     Reposition the alpaca
//    if (currentAction == Action::WALKING) {
//        if (currentDirection == Alpaca::Direction::RIGHT) {
//            angle += configGame->deltaTime * speed;
//        } else if (currentDirection == Alpaca::Direction::LEFT) {
//            angle -= configGame->deltaTime * speed;
//        }
//    }
//
//     Position calculation
//    x = configGame->calcX(angle);
//    y = configGame->calcY(angle);
Alpaca::Alpaca(b2World *world, float width, float height, float x, float y) : id(nextId++) {

    loadTextures();

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 15.0f;
    fixtureDef.restitution = 0.0f;

    b2CircleShape b2Shape;
    b2Shape.m_radius = width/2 / SCALE;
    fixtureDef.shape = &b2Shape;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void*) this);

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    /// SQUARE VERSION (SFML)
//    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
//    sfShape->setOrigin(width / 2, height / 2);
//    sfShape->setTexture(&alpacaTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);

    /// CIRCLE VERSION (SFML)
    sfShape = new sf::CircleShape(width/2);
    sfShape->setOrigin(width/2, width/2);
    sfShape->setTexture(&alpacaTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);


}

int Alpaca::nextId = 0;

int Alpaca::randomNumberGenerator(int lower, int upper) {
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(generator);
}

void Alpaca::loadTextures() {
    if (!alpacaTexture.loadFromFile("entity/alpaca/alpaca.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}

void Alpaca::adjust() {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));
}
