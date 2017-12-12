#include "HitPointBarometer.h"
#include "../../Resources/ConfigGame.h"


HitPointBarometer::HitPointBarometer(ConfigGame *configGame, int &HP, float width, float height) {
    this->configGame = configGame;

    this->Texture = configGame->healthTexture;
    this->HP = &HP;

    sfShape.setSize(sf::Vector2f(width,height));
    sfShape.setOrigin(sfShape.getSize().x/2, sfShape.getSize().y/2);
    sfShape.setTexture(&Texture);

    label_HP = configGame->createLabel(&configGame->fontID, 20, "HP: " + std::to_string(HP));

}

void HitPointBarometer::setPlacement(float x, float y, float angle) {

    label_HP.setPosition(x,y);
    label_HP.setRotation(angle);

    float offset = 5.f;
    sfShape.setPosition(label_HP.getTransform().transformPoint(sf::Vector2f(-sfShape.getSize().x + offset, label_HP.getLocalBounds().height/2)));
    sfShape.setRotation(angle);
}

void HitPointBarometer::render(sf::RenderWindow *window) {
    label_HP.setString("HP: " + std::to_string(*HP));
    window->draw(label_HP);
    window->draw(sfShape);
}
