#include "HitPointBarometer.h"
#include "../../Resources/ConfigGame.h"


HitPointBarometer::HitPointBarometer(ConfigGame *configGame,sf::Texture texture, std::string startHP, float width, float height)
{
    this->configGame = configGame;

    this->Texture = texture;
    this->HP = startHP;



    sfShape.setSize(sf::Vector2f(width,height));
    sfShape.setOrigin(sfShape.getSize().x/2, sfShape.getSize().y/2);
    sfShape.setTexture(&Texture);


    this->configGame->createLabel(this->label_HP, &this->configGame->fontID, HP);

}

void HitPointBarometer::setPlacement(float x, float y, float angle) {

    label_HP.setPosition(x,y);
    label_HP.setRotation(angle);

    float offset = 5.f;
    sfShape.setPosition(label_HP.getTransform().transformPoint(sf::Vector2f(-sfShape.getSize().x + offset, label_HP.getLocalBounds().height/2)));
    sfShape.setRotation(angle);
}

void HitPointBarometer::render(sf::RenderWindow *window, std::string HP) {

    label_HP.setString("HP: " + HP);
    window->draw(label_HP);
    window->draw(sfShape);
}