#pragma once

#include <SFML/Graphics.hpp>

class Clickable {
public:
	Clickable(const sf::Vector2f& size, const sf::Vector2f& position);
	~Clickable();

	void draw(sf::RenderWindow& window);
	const bool isMouseOver(const sf::Vector2f& mousePosition) const;

private:
	sf::RectangleShape shape;
};

