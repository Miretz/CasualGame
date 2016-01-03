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
	
	std::vector<sf::Uint32> m_texture[8];
	
	sf::Texture m_screenTex;
	sf::Sprite m_screenSprite;
	std::vector<std::vector<sf::Uint32> > m_buffer;
	sf::Uint8* m_screenPix;

	sf::Vector2f m_mousePos;

	float m_moveSpeed;
	float m_rotSpeed;

	bool m_movement[4] = {false, false, false, false};

};

