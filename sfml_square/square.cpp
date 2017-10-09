#include <SFML/Graphics.hpp>

#include "square.h"

Square::Square(float x, float y, const Config& config, sf::RenderWindow& window)
		: x(x), y(y),
		  config(config),
		  window(window)

{
	rectangle.setSize(sf::Vector2f(size, size));
	rectangle.setFillColor(sf::Color::Red);
}

void Square::move(float delta)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		x += speed *  delta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		x -= speed * delta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		y -= speed * delta;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		y += speed * delta;

	// Keep the box within screen borders
	x = std::max(x, 0.f);
	x = std::min(x, (float)(config.screenWidth - size));
	y = std::max(y, 0.f);
	y = std::min(y, (float)(config.screenHeight - size));
}

void Square::draw()
{
	// Position the rectangle used to draw the square
	rectangle.setPosition(x, y);

	// Draw the square
	window.draw(rectangle);
}
