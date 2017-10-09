#include "main_menu.h"

main_menu::main_menu()
{
    font.loadFromFile("font.ttf");

    menu[0].setFont(font);
    menu[0].setString("Play");
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setPosition(350, 150);

    menu[1].setFont(font);
    menu[1].setString("Option");
    menu[1].setFillColor(sf::Color::Green);
    menu[1].setPosition(350, 250);

    menu[2].setFont(font);
    menu[2].setString("Exit");
    menu[2].setFillColor(sf::Color::Green);
    menu[2].setPosition(350, 350);


}

void main_menu::moveUp()
{
    if (menuIndex - 1 >= 0)
    {
        menu[menuIndex].setFillColor(sf::Color::Green);
        menuIndex--;
        menu[menuIndex].setFillColor(sf::Color::Red);
    }

}

void main_menu::moveDown()
{
    if (menuIndex + 1 <= 2)
    {
        menu[menuIndex].setFillColor(sf::Color::Green);
        menuIndex++;
        menu[menuIndex].setFillColor(sf::Color::Red);
    }

}
