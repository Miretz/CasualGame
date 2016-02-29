#pragma once

#include <SFML/Graphics.hpp>

class Clickable {
public:
	Clickable();
	virtual ~Clickable() = default;

	void update(const sf::Vector2f& size, const sf::Vector2f& position);
	void draw(sf::RenderWindow* window);
	bool isMouseOver(const sf::Vector2f& mousePosition) const;
	bool isVisible() const { return m_visible;  };
	void setVisible(bool visible = true) { m_visible = visible; }

private:
	sf::RectangleShape m_shape;
	bool m_visible;
};

