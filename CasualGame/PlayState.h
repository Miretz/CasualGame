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

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	const static int m_level[24][24];

	std::unique_ptr<sf::Vector2f> m_pos;
	std::unique_ptr<sf::Vector2f> m_dir;
	std::unique_ptr<sf::Vector2f> m_plane;
	
	std::vector<sf::Uint32> m_texture[8];

	sf::Vertex* m_buffer;

	float m_moveSpeed;
	float m_rotSpeed;

	bool m_movement[4] = {false, false, false, false};

};

