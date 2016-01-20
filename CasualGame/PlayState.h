#pragma once

#include "GameState.h"
#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "Clickable.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

static const float minimapScale = 8.0f;
static const int minimapTransparency = 140;
static const double PI = 3.141592653589793238463;

class PlayState : public GameState {
public:
	PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	~PlayState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	bool m_forward = false;
	bool m_backward = false;
	bool m_left = false;
	bool m_right = false;

	const int m_windowWidth;
	const int m_windowHeight;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<LevelReaderWriter> m_levelReader;
	const std::vector<std::vector<int> >& m_levelRef;
	const std::vector<Sprite>& m_spriteRef;
	const size_t m_levelSize;
	const size_t m_spriteSize;
	sf::Vector2f m_mousePosition;
	
	std::vector<double> m_ZBuffer;

	//arrays used to sort the sprites
	std::vector<int> m_spriteOrder;
	std::vector<double> m_spriteDistance;

	std::vector<sf::Vertex> m_buffer;

	std::vector<Clickable> m_spriteOutlines;

	sf::Font m_font;
	sf::Text m_fpsDisplay;

	const unsigned int calculateWalls();
	const unsigned int calculateSprites();
	void drawMinimap(sf::RenderWindow* window);
	const sf::Color toColor(const sf::Uint32& colorRgba);
	void combSort(std::vector<int>& order, std::vector<double>& dist, int amount);

};