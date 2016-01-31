#include "Clickable.h"

Clickable::Clickable(const sf::Vector2f & size, const sf::Vector2f & position) : m_toggled(false) {
	m_shape.setSize(size);
	m_shape.setPosition(position);

	m_shape.setFillColor({ 255, 255, 255, 0 });
	m_shape.setOutlineColor({ 255, 255, 255, 0 });
}

Clickable::~Clickable() {
	//Empty
}

void Clickable::draw(sf::RenderWindow & window) {
	m_shape.setOutlineColor({ 255, 255, 255, 255 });
	m_shape.setOutlineThickness(2);
	window.draw(m_shape);
}

const bool Clickable::isMouseOver(const sf::Vector2f & mousePosition) const {
	return m_shape.getGlobalBounds().contains(mousePosition);
}


