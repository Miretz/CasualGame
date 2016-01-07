#pragma once

#include "GameState.h"
#include "Game.h"
#include "RandomGenerator.h"
#include "VectorUtils.h"

struct Sprite
{
	double x;
	double y;
	int texture;
};

static constexpr int numSprites = 19;
static constexpr int numTextures = 13;

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
	void combSort(int* order, double* dist, int amount);

	const static int m_level[24][24];
	const static Sprite m_sprites[numSprites];

	double m_posX;
	double m_posY;
	double m_dirX;
	double m_dirY;
	double m_planeX;
	double m_planeY;

	double m_ZBuffer[1024];

	//arrays used to sort the sprites
	int spriteOrder[numSprites];
	double spriteDistance[numSprites];

	std::vector<sf::Uint32> m_texture[numTextures];

	sf::Vertex* m_buffer;

	bool m_movement[4] = {false, false, false, false};

};

