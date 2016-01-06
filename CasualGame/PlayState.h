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

	void loadTexture(int index, const std::string& fileName);

	const static int m_level[24][24];

	double m_posX;
	double m_posY;
	double m_dirX;
	double m_dirY;
	double m_planeX;
	double m_planeY;

	std::vector<sf::Uint32> m_texture[10];

	sf::Vertex* m_buffer;

	bool m_movement[4] = {false, false, false, false};

};

