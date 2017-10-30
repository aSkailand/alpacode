#include "Farmer.h"

Farmer::Farmer(b2World *world, float width, float height, float x, float y) {

    this->x = x;
    this->y = y;

    loadTextures();

    // Create body definition
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->x / SCALE, this->y / SCALE);
    bodyDef.type = b2_dynamicBody;

    // Create body
    body = world->CreateBody(&bodyDef);

    // Create Fixture
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 15.0f;
    fixtureDef.restitution = 0.0f;

    b2CircleShape b2Shape;
    b2Shape.m_radius = width / 2 / SCALE;
    fixtureDef.shape = &b2Shape;

    // Store information
    setID(Entity::ID::ALPACA);
    body->SetUserData((void *) this);

    // Connect fixture to body
    body->CreateFixture(&fixtureDef);

    /// SQUARE VERSION (SFML)
//    sfShape = new sf::RectangleShape(sf::Vector2f(width, height));
//    sfShape->setOrigin(width / 2, height / 2);
//    sfShape->setTexture(&alpacaTexture);
//    sfShape->setOutlineThickness(2);
//    sfShape->setOutlineColor(sf::Color::Black);

    /// CIRCLE VERSION (SFML)
    sfShape = new sf::CircleShape(width / 2);
    sfShape->setOrigin(width / 2, width / 2);
    sfShape->setTexture(&farmerTexture);
    sfShape->setOutlineThickness(2);
    sfShape->setOutlineColor(sf::Color::Black);




//     Assigning pointers
//    configGame = &stateMachine.configGame;
//    window = &stateMachine.configWindow.getWindow();
//
//     Load textures
//    loadTextures();
//
    // Assigning default states
//    currentAction = Action::IDLE;
//    currentStatus = Status::GROUNDED;
//    currentDirection = Direction::RIGHT;

    // Define the alpaca
//    farmer = sf::RectangleShape(sf::Vector2f(size, size));
//    farmer.setTexture(&farmerTexture);
//    farmer.setOrigin(farmer.getSize().x / 2, farmer.getSize().y);
    //farmer.setOutlineThickness(1);
//    farmer.scale(-1.f, 1.f);

    // Assigning initial position
//    angle = initAngle;
}

//void Farmer::draw() {
//
//     Place the farmer accordingly
//    farmer.setPosition(x,y);
//
//     Draw the square
//    window->draw(farmer);
//}

/*
void Farmer::control() {

    // Move farmer according to input
    if (configGame->currentInput == sf::Keyboard::Right) {
        angle += configGame->deltaTime * speed;
    } else if (configGame->currentInput == sf::Keyboard::Left) {
        angle -= configGame->deltaTime * speed;
    }

    // todo: Simplify flipping?
    // Changes which way to flip the farmer
    if (configGame->currentInput == sf::Keyboard::Unknown) {
        currentAction = Action::IDLE;
    } else if (configGame->currentInput == sf::Keyboard::Right) {
        currentAction = Action::WALKING;
        if (currentDirection != Direction::RIGHT) {
            farmer.scale(-1.f, 1.f);
            currentDirection = Direction::RIGHT;
        }
    } else if (configGame->currentInput == sf::Keyboard::Left) {
        currentAction = Action::WALKING;
        if (currentDirection != Direction::LEFT) {
            farmer.scale(-1.f, 1.f);
            currentDirection = Direction::LEFT;
        }
    }

    // Update the position and rotation of farmer
    x = configGame->calcX(angle);
    y = configGame->calcY(angle);
    farmer.setRotation(angle);

}
*/

void Farmer::loadTextures() {
    if (!farmerTexture.loadFromFile("entity/player/farmer.png")) {
        std::cout << "Error loading file!" << std::endl;
    }
}

void Farmer::adjust() {
    x = SCALE * body->GetPosition().x;
    y = SCALE * body->GetPosition().y;
    sfShape->setPosition(x, y);
    sfShape->setRotation((body->GetAngle() * DEGtoRAD));
}
