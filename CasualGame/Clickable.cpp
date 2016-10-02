#include "Clickable.h"

Clickable::Clickable() : m_visible(false), m_destructible(false), m_spriteIndex(-1)
{
	//Empty
}

void Clickable::update(const sf::Vector2f & size, const sf::Vector2f & position)
{
	m_shape.setSize(size);
	m_shape.setPosition(position);
	m_shape.setFillColor({ 255, 255, 255, 0 });
	m_shape.setOutlineColor({ 255, 255, 255, 0 });
}

void Clickable::draw(sf::RenderWindow* window)
{
	if (m_visible)
	{
		m_shape.setOutlineColor({ 255, 255, 255, 255 });
		m_shape.setOutlineThickness(1);
		window->draw(m_shape);
	}
}

bool Clickable::containsVector(const sf::Vector2f & position) const
{
	return m_shape.getGlobalBounds().contains(position);
}
