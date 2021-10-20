#pragma once

#include <memory>

#include "GameState.h"

struct Player;
class PlayerInputManager;
class GLRaycaster;
class LevelReaderWriter;

class PlayState : public GameState
{
public:
	PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);

	void update(const float ft, Game& game) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousePosition, Game& game) override;

private:

	size_t m_levelSize;
	size_t m_spriteSize;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<LevelReaderWriter> m_levelReader;

	std::unique_ptr<PlayerInputManager> m_inputManager;
	std::unique_ptr<GLRaycaster> m_glRaycaster;

	double m_runningTime = 0.0;

	//Gui	
	sf::Text m_fpsDisplay;
	sf::Text m_playerHealthDisplay;
	sf::RectangleShape m_gunDisplay;
	sf::CircleShape m_crosshair;
	sf::Texture m_textureGun;
	sf::Texture m_textureGun_fire;

	//Minimap
	std::vector<sf::RectangleShape> m_minimapWallBuffer;
	std::vector<sf::CircleShape> m_minimapEntityBuffer;
	sf::RectangleShape m_minimapBackground;
	sf::ConvexShape m_minimapPlayer;
	
	void generateMinimap();
	void updateMinimapEntities();
	void drawMinimap(sf::RenderWindow& window) const;
	void drawGui(sf::RenderWindow& window) const;

	void destroyAimedAtSprite() const;

};
