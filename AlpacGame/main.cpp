
#include <iostream>
#include <plog\log.h>
#include <SFML/Graphics.hpp>
#include "state/StateMachine.h"
#include "window.h"
#include "main_menu.h"

int main()
{

    plog::init(plog::debug,"Log.log");
    LOGI << "Initiating plog.";

    StateMachine stateMachine;

    while (stateMachine.isRunning){
        std::cout << "Machine is Proceeding:" << std::endl;
        stateMachine.proceed();
    }

    main_menu menu;

    sf::RenderWindow window(sf::VideoMode(800, 600), "AlpacGame");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::KeyPressed : {
                    switch (event.key.code) {
                        case sf::Keyboard::Up : {
                            menu.moveUp();
                            break;
                        }
                        case sf::Keyboard::Down : {
                            menu.moveDown();
                            break;
                        }
                        case sf::Keyboard::Return :
                        {
                            switch (menu.GetPressedItem())
                            {
                                case 0 :
                                std::cout << "Play" << std::endl;
                                    break;

                                case 1 :
                                std::cout << "Options" << std::endl;
                                    break;

                                case 2 :
                                window.close();

                            }
                        }
                    }
                    break;
                }
            }


            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);


        window.draw(menu.menu[0]);
        window.draw(menu.menu[1]);
        window.draw(menu.menu[2]);


        window.display();
    }
}