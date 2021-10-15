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
	virtual ~GLRaycaster() = default;

	void initialize(const int windowWidth, const int windowHeight, const int spriteSize);
	void calculateWalls(const Player& player, const LevelReaderWriter& levelReader);
	void calculateSprites(const Player& player, const LevelReaderWriter& levelReader);
	void setPixel(int x, int y, const sf::Uint32 colorRgba, unsigned int style);
	void draw(const Player& player, const LevelReaderWriter& levelReader);
	void bindGlBuffers() const;
	void cleanup() const;

	std::vector<Clickable>& getClickables() { return m_clickables; }

private:

	void drawSpriteVerticalStripe(int x, int start, int end, int spriteHeight, int texX, const std::vector<sf::Uint32>& textureData);

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	std::unique_ptr<GLRenderer> m_glRenderer;

	// buffer of clickable items in the view
	std::vector<Clickable> m_clickables;

	//main rendering buffer
	std::vector<unsigned char> m_buffer;

	// Z buffer for sprites
	std::vector<double> m_ZBuffer;

};

