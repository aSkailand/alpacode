#include <SFML/Window.hpp>

#include "app.h"

App::App() : square(200, 200, config, window)
{
	// Create and configure the window
	window.create(sf::VideoMode(config.screenWidth, config.screenHeight), "The Magic Square");

	// Locks the screen refresh to the monitor refresh (usually 60 fps)
	window.setVerticalSyncEnabled(true);

	clock.restart();
}

bool App::Tick()
{
	sf::Event event;
	float delta = clock.restart().asSeconds();

	// Get events from OS
	while (window.pollEvent(event))
	{
		// Close the window if the user clicks the x button
		if (event.type == sf::Event::Closed)
		{
			window.close();
			return false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		window.close();
		return false;
	}

	square.move(delta);

	window.clear(sf::Color::Black);
	square.draw();

	// Flip buffers to show what we've drawn
	window.display();

	return true;
}
