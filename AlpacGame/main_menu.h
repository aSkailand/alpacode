#ifndef ALPACGAME_MAIN_MENU_H
#define ALPACGAME_MAIN_MENU_H


#include <SFML/Graphics.hpp>


class main_menu
{
public:

    main_menu();

    sf::Text menu[3];

    void moveUp();
    void moveDown();

    int GetPressedItem() { return menuIndex; }


private:
    sf::Font font;

    int menuIndex = 0;




};


#endif //ALPACGAME_MAIN_MENU_H
