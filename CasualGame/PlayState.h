#pragma once

#include "GameState.h"
#include "Game.h"
#include "RandomGenerator.h"
#include "VectorUtils.h"

class PlayState : public GameState
{
public:
	PlayState();
	~PlayState();

	void update(float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:
	static RandomGenerator gen;

	sf::Vector2f m_mousePos;

	std::vector<sf::Color> m_bgColors;
	std::vector<sf::CircleShape> m_followers;

};

