#ifndef SFML_SQUARE_APP_H
#define SFML_SQUARE_APP_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "config.h"
#include "square.h"

class App
{
public:
	App();
	bool Tick();

protected:
	Config config;
	sf::RenderWindow window;
	Square square;

	sf::Clock clock;
};

#endif
