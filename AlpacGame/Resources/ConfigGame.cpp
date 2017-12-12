
#include <cmath>
#include <iostream>
#include "ConfigGame.h"
#include "../entity/CollisionListener.h"
#include "../entity/planet/planet.h"
#include "../scenery/Sun/Sun.h"
#include "../scenery/Sky/Sky.h"

#include "../scenery/Trees/TreeHigh.h"
#include "../scenery/Trees/TreeMedium.h"
#include "../scenery/Trees/TreeLow.h"
#include "../scenery/Cave/Cave.h"
#include "../scenery/Stone/Stone.h"


void ConfigGame::run(sf::RenderWindow &window) {

    this->window = &window;

    planetCenter = sf::Vector2f(window.getSize().x / 2, window.getSize().y);

    mouseArrow.setRadius(5);
    mouseArrow.setFillColor(sf::Color::Red);
    mouseArrow.setOutlineColor(sf::Color::Black);
    mouseArrow.setOutlineThickness(5);

    loadAllTextures();
    loadAllFonts();
    dayCounterLabel();
    alpacaCounterLabel();

    // Randomize number generator
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::default_random_engine(seed);

}


float ConfigGame::calcX(float angle) {
    float x = planetCenter.x + planetRadius * (float) std::sin(angle * (M_PI / 180));
    return x;
}

float ConfigGame::calcY(float degree) {
    float y = planetCenter.y - planetRadius * (float) std::cos(degree * (M_PI / 180));
    return y;
}

float ConfigGame::calcX(float degree, float radius) {
    float x = planetCenter.x + radius * (float) std::sin(degree * (M_PI / 180));
    return x;
}

float ConfigGame::calcY(float degree, float radius) {
    float y = planetCenter.y - radius * (float) std::cos(degree * (M_PI / 180));
    return y;
}

void ConfigGame::loadAllFonts() {
    fontID.loadFromFile("Resources/fontPixel.ttf");
}

void ConfigGame::loadTexture(std::string filename,
                             std::map<EntityWarm::Action, SpriteInfo> &spriteMap,
                             int width,
                             int height,
                             int top,
                             EntityWarm::Action action) {

    for (int x = spriteMap[action].startFrame; x <= spriteMap[action].endFrame; ++x) {

        auto *texture = new sf::Texture();

        if (!texture->loadFromFile(filename, sf::IntRect(x * width, top, width, height))) {
            std::cout << "Error loading file!" << std::endl;
        }

        spriteMap[action].sprites.push_back(texture);
    }
}

void ConfigGame::loadAllTextures() {

    // Sky
    sf::Texture morning_1;
    sf::Texture morning_2;
    sf::Texture morning_3;
    sf::Texture afternoon_4;
    sf::Texture afternoon_5;
    sf::Texture afternoon_6;
    sf::Texture evening_7;
    sf::Texture evening_8;
    sf::Texture evening_9;
    sf::Texture night_10;
    sf::Texture night_11;
    sf::Texture night_12;

    morning_1.loadFromFile("scenery/Sky/Background/1_Morning.png");
    morning_2.loadFromFile("scenery/Sky/Background/2_Morning.png");
    morning_3.loadFromFile("scenery/Sky/Background/3_Morning.png");
    afternoon_4.loadFromFile("scenery/Sky/Background/4_Afternoon.png");
    afternoon_5.loadFromFile("scenery/Sky/Background/5_Afternoon.png");
    afternoon_6.loadFromFile("scenery/Sky/Background/6_Afternoon.png");
    evening_7.loadFromFile("scenery/Sky/Background/7_Evening.png");
    evening_8.loadFromFile("scenery/Sky/Background/8_Evening.png");
    evening_9.loadFromFile("scenery/Sky/Background/9_Evening.png");
    night_10.loadFromFile("scenery/Sky/Background/10_Night.png");
    night_11.loadFromFile("scenery/Sky/Background/11_Night.png");
    night_12.loadFromFile("scenery/Sky/Background/12_Night.png");

    skyTextures.push_back(morning_1);
    skyTextures.push_back(morning_2);
    skyTextures.push_back(morning_3);
    skyTextures.push_back(afternoon_4);
    skyTextures.push_back(afternoon_5);
    skyTextures.push_back(afternoon_6);
    skyTextures.push_back(evening_7);
    skyTextures.push_back(evening_8);
    skyTextures.push_back(evening_9);
    skyTextures.push_back(night_10);
    skyTextures.push_back(night_11);
    skyTextures.push_back(night_12);

    // Planet
    sf::Texture earth_1;
    sf::Texture earth_2;
    sf::Texture earth_3;
    sf::Texture earth_4;
    sf::Texture earth_5;
    sf::Texture earth_6;
    sf::Texture earth_7;
    sf::Texture earth_8;
    sf::Texture earth_9;
    sf::Texture earth_10;
    sf::Texture earth_11;
    sf::Texture earth_12;

    earth_1.loadFromFile("entity/planet/Earth/Earth_1.png");
    earth_2.loadFromFile("entity/planet/Earth/Earth_2.png");
    earth_3.loadFromFile("entity/planet/Earth/Earth_3.png");
    earth_4.loadFromFile("entity/planet/Earth/Earth_4.png");
    earth_5.loadFromFile("entity/planet/Earth/Earth_5.png");
    earth_6.loadFromFile("entity/planet/Earth/Earth_6.png");
    earth_7.loadFromFile("entity/planet/Earth/Earth_7.png");
    earth_8.loadFromFile("entity/planet/Earth/Earth_8.png");
    earth_9.loadFromFile("entity/planet/Earth/Earth_9.png");
    earth_10.loadFromFile("entity/planet/Earth/Earth_10.png");
    earth_11.loadFromFile("entity/planet/Earth/Earth_11.png");
    earth_12.loadFromFile("entity/planet/Earth/Earth_12.png");

    planetTextures.push_back(earth_1);
    planetTextures.push_back(earth_2);
    planetTextures.push_back(earth_3);
    planetTextures.push_back(earth_4);
    planetTextures.push_back(earth_5);
    planetTextures.push_back(earth_6);
    planetTextures.push_back(earth_7);
    planetTextures.push_back(earth_8);
    planetTextures.push_back(earth_9);
    planetTextures.push_back(earth_10);
    planetTextures.push_back(earth_11);
    planetTextures.push_back(earth_12);

    // Trees
    int treeWidth = 128;
    int treeHeight = 272;
    treeHighTexture.loadFromFile("scenery/Trees/trees.png", sf::IntRect(0, 0, treeWidth, treeHeight));
    treeMediumTexture.loadFromFile("scenery/Trees/trees.png", sf::IntRect(treeWidth, 0, treeWidth, treeHeight));
    treeLowTexture.loadFromFile("scenery/Trees/trees.png", sf::IntRect(2*treeWidth, 0, treeWidth, treeHeight));

    //Stone
    stoneTexture.loadFromFile("scenery/Stone/stone.png");

    // Wolf Cave
    wolfCaveTexture.loadFromFile("scenery/Cave/cave.png");

    // Sun/Moon
    sf::Texture sun_1;
    sf::Texture sun_2;
    sf::Texture moon_1;

    sun_1.loadFromFile("scenery/Sun/Sun_1.png");
    sun_2.loadFromFile("scenery/Sun/Sun_2.png");
    moon_1.loadFromFile("scenery/Sun/Moon_1.png");

    sunTextures.push_back(sun_1);
    sunTextures.push_back(sun_2);
    sunTextures.push_back(moon_1);

    // Shotgun
    shotgunHeldTexture.loadFromFile("entity/shotgun/shotgunHeld.png");
    shotgunDropTexture.loadFromFile("entity/shotgun/shotgunDrop.png");

    // Trap
    int trapWidth = 50;
    int trapHeight = 25;
    trapOpenTexture.loadFromFile("entity/trap/trap.png", sf::IntRect(0, 0, trapWidth, trapHeight));
    trapClosedTexture.loadFromFile("entity/trap/trap.png", sf::IntRect(trapWidth, 0, trapWidth, trapHeight));

    // Cooldown Barometer
    int cooldownWidth = 21;
    int cooldownHeight = 21;
    for (int i = 0; i < 5; ++i) {
        auto *tempTexture = new sf::Texture();
        tempTexture->loadFromFile("entity/cooldown/cooldown.png",
                                  sf::IntRect(i * cooldownWidth, 0, cooldownWidth, cooldownHeight));
        cooldownTextures.push_back(tempTexture);
    }

    // Shotgun Bullets Indicator
    int indicatorWidth = 280;
    int indicatorHeight = 280;
    for (int i = 0; i < 3; ++i) {
        auto tempTexture = sf::Texture();
        tempTexture.loadFromFile("entity/Shotgun/shotgun-bullet.png",
                                 sf::IntRect(i * indicatorWidth, 0, indicatorWidth, indicatorHeight));
        bulletIndicatorTextures.push_back(tempTexture);
    }

    // Alert Indicator
    alertRedTexture.loadFromFile("entity/Alpaca/redAlert.png");
    alertYellowTexture.loadFromFile("entity/Alpaca/yellowAlert.png");

    // Alert Wolf Indicator
    alertSteakTexture.loadFromFile("entity/Wolf/steak1.png");
    alertFlagTexture.loadFromFile("entity/Wolf/flag.png");

    // HitPoint
    healthTexture.loadFromFile("entity/HitPoint/HealthCross.png");

    fertileHeartTexture.loadFromFile("entity/HitPoint/heart.png");

    // Farmer
    std::string farmerSpriteSheet = "entity/player/farmer-sprite.png";

    int farmerWidth = 50;
    int farmerHeight = 70;

    farmerSprites[EntityWarm::Action::IDLE].startFrame = 0;
    farmerSprites[EntityWarm::Action::IDLE].endFrame = 3;
    loadTexture(farmerSpriteSheet, farmerSprites, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::IDLE);

    farmerSprites[EntityWarm::Action::WALKING].startFrame = 4;
    farmerSprites[EntityWarm::Action::WALKING].endFrame = 9;
    loadTexture(farmerSpriteSheet, farmerSprites, farmerWidth, farmerHeight, 0,
                EntityWarm::Action::WALKING);

    // Alpaca
    std::string alpacaSpriteSheet = "entity/alpaca/alpaca-sprite.png";

    int alpacaWidth = 100;
    int alpacaHeight = 110;

    alpacaSprites[EntityWarm::Action::IDLE].startFrame = 0;
    alpacaSprites[EntityWarm::Action::IDLE].endFrame = 4;
    loadTexture(alpacaSpriteSheet, alpacaSprites, alpacaWidth, alpacaHeight, 0,
                EntityWarm::Action::IDLE);

    alpacaSprites[EntityWarm::Action::WALKING].startFrame = 0;
    alpacaSprites[EntityWarm::Action::WALKING].endFrame = 1;
    loadTexture(alpacaSpriteSheet, alpacaSprites, alpacaWidth, alpacaHeight, 0,
                EntityWarm::Action::WALKING);


    // Wolf
    std::string wolfSpriteSheet1 = "entity/wolf/wolf-sprite.png";

    int wolfWidth = 145;
    int wolfHeight = 80;

    wolfSprites[EntityWarm::Action::IDLE].startFrame = 0;
    wolfSprites[EntityWarm::Action::IDLE].endFrame = 3;
    loadTexture(wolfSpriteSheet1, wolfSprites, wolfWidth, wolfHeight, 0,
                EntityWarm::Action::IDLE);

    wolfSprites[EntityWarm::Action::WALKING].startFrame = 1;
    wolfSprites[EntityWarm::Action::WALKING].endFrame = 2;
    loadTexture(wolfSpriteSheet1, wolfSprites, wolfWidth, wolfHeight, 0,
                EntityWarm::Action::WALKING);

}

void ConfigGame::reset() {

    // Initiating World (With no innate gravitation)
    delete world;
    world = new b2World(b2Vec2(0, 0));
    world->SetContactListener(new CollisionListener());

    // Instantiating initial entities
    delete planet;
    planet = new Planet(this, planetRadius, planetCenter.x, planetCenter.y);

    farmer = nullptr;
    delete farmer;
    farmer = new Farmer(this, 30, 100, 100, calcX(0), calcY(0));

    /// Create Vectors
    delete entities;
    entities = new std::vector<Entity *>;
    entities->push_back(planet);
    entities->push_back(farmer);

    entities->push_back(new Shotgun(this, 100, 25, calcX(5.f), calcY(5.f)));
    entities->push_back(new Trap(this, 150, 75, calcY(-5.f), calcY(-5.f)));
    entities->push_back(new Trap(this, 150, 75, calcY(-10.f), calcY(-10.f)));

    entities->push_back(new Alpaca(this, true, calcX(20.f), calcY(20.f)));
    entities->push_back(new Alpaca(this, true, calcX(10.f), calcY(10.f)));
    entities->push_back(new Alpaca(this, true, calcX(-10.f), calcY(-10.f)));
    entities->push_back(new Alpaca(this, true, calcX(-20.f), calcY(-20.f)));

    // Instantiating initial scenery
    delete sceneries;
    sceneries = new std::vector<Scenery *>;

    delete sky;
    sky = new Sky(this, 4500.f, planetCenter.x, planetCenter.y);
    sceneries->push_back(sky);

    delete sun;
    sun = new Sun(this, sunRadius, calcX(0.f, sunRadius), calcY(0.f, sunRadius), 0.f);
    sceneries->push_back(sun);


    std::uniform_int_distribution<int> distributionAmount(5, 8);
    treeHighAmount = (int) distributionAmount(generator);
    treeMediumAmount = (int) distributionAmount(generator);
    treeLowAmount = (int) distributionAmount(generator);
    stoneAmount = (int) distributionAmount(generator);

    std::uniform_int_distribution<int> distribution(0, 359);
    for (int i = 0; i < treeHighAmount; i++) {
        treeHighAngle = (float) distribution(generator);
        sceneries->push_back(new TreeHigh(this, treeHighWidth, treeHighHeight, treeHighAngle));
    }


    for (int i = 0; i < treeMediumAmount; i++) {
    treeMediumAngle = (float) distribution(generator);
    sceneries->push_back(new TreeMedium(this, treeMediumWidth, treeMediumHeight, treeMediumAngle));
    }


    for(int i = 0; i<treeLowAmount; i++) {
        treeLowAngle = (float) distribution(generator);
        sceneries->push_back(new TreeLow(this, treeLowWidth, treeLowHeight, treeLowAngle));
    }

    for(int i = 0; i< stoneAmount; i++){
        stoneAngle = (float) distribution(generator);
        sceneries->push_back(new Stone(this,stoneWidth,stoneHeight,stoneAngle));
    }


    cave = new Cave(this, 200.f, 150.f);
    dynamic_cast<Cave*>(cave)->reposition(wolfDenAngle);
    sceneries->push_back(cave);

    // Initiate dayCycle
    delete dayCycle;
    dayCycle = new DayCycle(this);

    // Set ID
    Wolf::nextId = 0;
    Alpaca::nextId = 0;

    // Reset Game Stats
    numOfDay = 1;

}

ConfigGame::Cycle ConfigGame::getCurrentCycle() const {
    return currentCycle;
}

void ConfigGame::setCurrentCycle(ConfigGame::Cycle currentCycle) {
    ConfigGame::currentCycle = currentCycle;
}

sf::Text ConfigGame::createLabel(sf::Font *font, unsigned int fontSize, const std::string &text) {
    sf::Text label;
    label.setFont(*font);
    label.setCharacterSize(fontSize);
    label.setFillColor(sf::Color::White);
    label.setOutlineColor(sf::Color::Black);
    label.setOutlineThickness(3);
    label.setString(text);
    label.setOrigin(label.getLocalBounds().width / 2.f, label.getLocalBounds().height / 2.f);
    return label;
}

void ConfigGame::dayCounterLabel() {
    dayLabel = tgui::Label::create();
    dayLabel->setPosition(5, 5);
    dayLabel->setSize(500, 100);
    dayLabel->setTextSize(38);
    dayLabel->setTextStyle(sf::Text::Bold);
    dayLabel->setFont(this->fontID);
    dayLabel->setText("");
}

void ConfigGame::alpacaCounterLabel() {
    alpacaCounter = tgui::Label::create();
    alpacaCounter->setPosition(5, 55);
    alpacaCounter->setSize(500, 100);
    alpacaCounter->setTextSize(24);
    alpacaCounter->setTextColor(sf::Color::White);
    alpacaCounter->setFont(this->fontID);
    alpacaCounter->setText("");
}

void ConfigGame::setDayCounter(std::string value) {
    dayLabel->setText("Day " + value);
}

void ConfigGame::setAlpacaCounter(std::string value) {
    alpacaCounter->setText("Alpacas: " + value);
}

tgui::Label::Ptr ConfigGame::getDayLabel() {
    return dayLabel;
}

tgui::Label::Ptr ConfigGame::getAlpacaLabel() {
    return alpacaCounter;
}

void ConfigGame::initiateNewDay() {

    // Increment days
    numOfDay++;

    /// Morning alpaca check
    for(Entity* entity : *entities){
        if(entity->getEntity_ID() == Entity::ID::ALPACA){

            // Retrieve alpaca
            auto *alpacaPtr =  dynamic_cast<Alpaca*>(entity);

            // Fertilize and grow alpacas
            if(alpacaPtr->isAdult){
                alpacaPtr->isFertile = true;
            } else{
                alpacaPtr->adultify();
            }

            // Heal alpaca to max
            alpacaPtr->HP = alpacaPtr->max_HP;
        }
    }

    dynamic_cast<Farmer*>(farmer)->HP = dynamic_cast<Farmer*>(farmer)->max_HP;

    // Update wolf den position
    std::uniform_int_distribution<int> distribution(0, 359);
    wolfDenAngle = (float) distribution(generator);
    wolfDenPos = sf::Vector2f(
            calcX(wolfDenAngle),
            calcY(wolfDenAngle));

    // Spawn Cave
    dynamic_cast<Cave*>(cave)->reposition(wolfDenAngle);

}

void ConfigGame::initiateNight() {
    spawnWolves = true;
    wolfSpawnTimer.reset(true);
    maxWolves = numOfDay * 2;

}

