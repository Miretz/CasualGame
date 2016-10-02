#pragma once

#include <vector>
#include <math.h> 
#include <iostream> 

#include "GameState.h"
#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "GLRenderer.h"
#include "Clickable.h"

class PlayState : public GameState
{
public:
	PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	virtual ~PlayState() = default;

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f mousePosition, Game& game) override;

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

	std::vector<double> m_ZBuffer;

	//arrays used to sort the sprites
	std::vector<int> m_spriteOrder;
	std::vector<double> m_spriteDistance;

	//main rendering buffer
	std::vector<unsigned char> m_buffer;

	// buffer of clickable items in the view
	std::vector<Clickable> m_clickables;

	//Gui	
	sf::Font m_font;
	sf::Text m_fpsDisplay;
	sf::Text m_playerHealthDisplay;
	sf::RectangleShape m_gunDisplay;
	sf::CircleShape m_crosshair;
	sf::Texture m_textureGun;
	sf::Texture m_textureGun_fire;

	double m_runningTime = 0.0;
	double m_shotTime = -1.0;
	double m_gunShotDelay = -1.0;

	float m_mouseDeltaFromCenter;

	//Gl renderer
	GLRenderer m_glRenderer;

	void calculateWalls();
	void calculateSprites();
	void drawMinimap(sf::RenderWindow* window) const;
	void drawGui(sf::RenderWindow* window);
	void setPixel(int x, int y, const sf::Uint32 colorRgba, int style);
	void combSort(std::vector<int>& order, std::vector<double>& dist, int amount) const;
	void handleShot();
	void handleMouselook(const sf::Vector2f mouseMovePos);
	void calculateShotTime(double fts);
};
