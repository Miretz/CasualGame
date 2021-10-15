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
	void bindGlBuffers() const;
	void cleanup() const;

	std::vector<Clickable>& getClickables() { return m_clickables; }

private:

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	std::unique_ptr<GLRenderer> m_glRenderer;

	// buffer of clickable items in the view
	std::vector<Clickable> m_clickables;

	//it makes sense for these to be raw pointers
	//main rendering buffer
	unsigned char* m_buffer = nullptr;

	// Z buffer for sprites
	double* m_ZBuffer;



};

