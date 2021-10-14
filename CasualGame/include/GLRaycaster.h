#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Game;
class GLRenderer;
class Clickable;
class LevelReaderWriter;
struct Player;

class GLRaycaster
{
public:
	GLRaycaster();
	virtual ~GLRaycaster();

	void initialize(const int windowWidth, const int windowHeight, const int spriteSize);
	void calculateWalls(const Player& player, const LevelReaderWriter& levelReader);
	void calculateSprites(const Player& player, const LevelReaderWriter& levelReader);
	void setPixel(int x, int y, const sf::Uint32 colorRgba, unsigned int style);
	void draw(const Player& player, const LevelReaderWriter& levelReader);
	void bindGlBuffers();
	void cleanup();

	std::vector<Clickable>& getClickables() { return m_clickables; }

private:

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	std::unique_ptr<GLRenderer> m_glRenderer;

	std::vector<double> m_ZBuffer;

	//arrays used to sort the sprites
	std::vector<int> m_spriteOrder;
	std::vector<double> m_spriteDistance;

	//main rendering buffer
	std::vector<unsigned char> m_buffer;

	// buffer of clickable items in the view
	std::vector<Clickable> m_clickables;

};

