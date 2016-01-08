#pragma once

#include "GameState.h"
#include "Game.h"
#include "RandomGenerator.h"
#include "VectorUtils.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Sprite
{
	double x;
	double y;
	int texture;
};

static constexpr int numSprites = 19;
static constexpr int numTextures = 13;
static constexpr int mapSize = 24;
static constexpr int minimapScale = 8;
static constexpr int minimapTransparency = 140;
static constexpr auto levelFile = "resources/levels/level1.txt";

static const double PI = 3.141592653589793238463;

class PlayState : public GameState
{
public:
	PlayState();
	~PlayState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	void loadLevel();
	void generateTextures();
	void loadTexture(int index, const std::string& fileName);
	void combSort(int* order, double* dist, int amount);

	int m_level[mapSize][mapSize];
	
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

