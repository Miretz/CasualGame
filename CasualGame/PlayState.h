#pragma once

#include "GameState.h"
#include "Game.h"
#include "RandomGenerator.h"
#include "VectorUtils.h"

struct LineData {
	int x;
	int drawStart;
	int drawEnd;
	sf::Color color;
};

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

	const static int m_level[24][24];
	
	sf::Vector2f m_pos;
	sf::Vector2f m_dir;
	sf::Vector2f m_plane;

	std::vector<LineData> m_lines;
	sf::Vector2f m_mousePos;
	std::vector<sf::Color> m_bgColors;

	float m_moveSpeed;
	float m_rotSpeed;

	bool m_movement[4] = {false, false, false, false};

};

