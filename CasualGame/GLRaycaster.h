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
	~GLRaycaster();

	void initialize(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	void calculateWalls(const int windowWidth, const int windowHeight);
	void calculateSprites(const int windowWidth, const int windowHeight);
	void setPixel(int x, int y, const sf::Uint32 colorRgba, int style, int windowWidth, int windowHeight);
	void draw(const int windowWidth, const int windowHeight);
	void bindGlBuffers();
	void cleanup();

	std::vector<Clickable>& getClickables() { return m_clickables; }

private:

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

