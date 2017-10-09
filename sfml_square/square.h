#ifndef SFML_SQUARE_SQUARE_H
#define SFML_SQUARE_SQUARE_H

#include <SFML/Graphics.hpp>

#include "config.h"

// Simple structure (a class where everything is public) for our square
struct Square
{
	Square(float x, float y, const Config& config, sf::RenderWindow& window);
	void move(float delta);
	void draw();

	const int size = 100;
	const float speed = 400;
	float x, y;

	sf::RectangleShape rectangle;

	// Dependencies
	const Config& config;
	sf::RenderWindow& window;
};

#endif
