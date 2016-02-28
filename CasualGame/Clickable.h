#pragma once

#include <SFML/Graphics.hpp>

class Clickable {
public:
	Clickable(const sf::Vector2f& size, const sf::Vector2f& position);
	~Clickable();

	void draw(sf::RenderWindow& window);
	
	bool isMouseOver(const sf::Vector2f& mousePosition) const;
	bool isToggled() const { return m_toggled;  };
	void toggle() { m_toggled = !m_toggled; }

private:
	sf::RectangleShape m_shape;
	bool m_toggled;
};

