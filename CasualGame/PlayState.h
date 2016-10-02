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
#include "GLRaycaster.h"
#include "Clickable.h"
#include "PlayerInputManager.h"
#include "Utils.h"

class PlayState : public GameState
{
public:
	PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	virtual ~PlayState() = default;

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f mousePosition, Game& game) override;

private:

	size_t m_levelSize;
	size_t m_spriteSize;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<LevelReaderWriter> m_levelReader;

	PlayerInputManager m_inputManager;

	double m_runningTime = 0.0;

	//Gui	
	sf::Font m_font;
	sf::Text m_fpsDisplay;
	sf::Text m_playerHealthDisplay;
	sf::RectangleShape m_gunDisplay;
	sf::CircleShape m_crosshair;
	sf::Texture m_textureGun;
	sf::Texture m_textureGun_fire;

	GLRaycaster m_glRaycaster;

	void drawMinimap(sf::RenderWindow* window) const;
	void drawGui(sf::RenderWindow* window);

};
