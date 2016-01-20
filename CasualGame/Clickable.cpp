#include "Clickable.h"

Clickable::Clickable(const sf::Vector2f & size, const sf::Vector2f & position) {
	shape.setSize(size);
	shape.setPosition(position);

	shape.setFillColor({ 255, 255, 255, 0 });
	shape.setOutlineColor({ 255, 255, 255, 0 });
}

Clickable::~Clickable() {
	//Empty
}

void Clickable::draw(sf::RenderWindow & window) {
	shape.setOutlineColor({ 255, 255, 255, 255 });
	shape.setOutlineThickness(2);
	window.draw(shape);
}

const bool Clickable::isMouseOver(const sf::Vector2f & mousePosition) const {
	return shape.getGlobalBounds().contains(mousePosition);
}


