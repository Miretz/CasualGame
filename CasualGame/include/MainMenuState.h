#pragma once

#include "GameState.h"

class Game;
class RandomGenerator;

class MainMenuState : public GameState
{
public:
	MainMenuState(const int w, const int h);
	~MainMenuState() override = default;

	void update(const float ft, Game& game) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousePosition, Game& game) override;

private:

	static RandomGenerator gen;

	const int m_windowWidth;
	const int m_windowHeight;

	std::vector<sf::Text> m_menuItems;
	sf::Text m_titleText;
	sf::Vector2f m_mousePos;

	unsigned int m_mouseOverIndex = 0;

	std::vector<sf::Color> m_bgColors;
	std::vector<sf::CircleShape> m_followers;

};

