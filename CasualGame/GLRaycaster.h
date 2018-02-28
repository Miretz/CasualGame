#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Game;
class GLRenderer;
class Clickable;
class LevelReaderWriter;
class Player;

class GLRaycaster
{
public:
	GLRaycaster();
	virtual ~GLRaycaster();

	void initialize(const int windowWidth, const int windowHeight, 
		std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	void calculateWalls();
	void calculateSprites();
	void setPixel(int x, int y, const sf::Uint32 colorRgba, int style);
	void draw();
	void bindGlBuffers();
	void cleanup();

	std::vector<Clickable>& getClickables() { return m_clickables; }

private:

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	std::unique_ptr<GLRenderer> m_glRenderer;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<LevelReaderWriter> m_levelReader;
	size_t m_spriteSize;

	std::vector<double> m_ZBuffer;

	//arrays used to sort the sprites
	std::vector<int> m_spriteOrder;
	std::vector<double> m_spriteDistance;

	//main rendering buffer
	std::vector<unsigned char> m_buffer;

	// buffer of clickable items in the view
	std::vector<Clickable> m_clickables;

};

