#pragma once

#include "GameState.h"
#include "Game.h"
#include "RandomGenerator.h"
#include "VectorUtils.h"

class MainMenuState : public GameState
{
public:
	MainMenuState();
	~MainMenuState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	static RandomGenerator gen;

	sf::Font m_font;
	std::vector<sf::Text> m_menuItems;
	sf::Text m_titleText;
	
	sf::Vector2f m_mousePos;

	const float m_padding = 20.0f;
	int m_mouseOverIndex = 0;

	std::vector<sf::Color> m_bgColors;
	std::vector<sf::CircleShape> m_followers;

};

