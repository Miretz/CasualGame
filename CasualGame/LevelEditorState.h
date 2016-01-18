#pragma once

#include "GameState.h"
#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "LevelEditorGui.h"

class LevelEditorState : public GameState {
public:
	LevelEditorState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	~LevelEditorState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	const int m_windowWidth;
	const int m_windowHeight;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<LevelReaderWriter> m_levelReader;

	float m_scale;
	
	bool m_filenameMode = false;
	int m_filenameGuiIndex;
	int m_selectedTexture = 1;
	int m_textureButtonId;
	int m_selectedSprite = 11;
	int m_spriteButtonId;
	bool m_editEntities = false;
	int m_entitySelected = -1;
	
	sf::Font m_font;
	sf::Text m_statusBar;
	sf::Vector2f m_mousePos;
	std::string m_customLevelName = "<enter filename>";
	std::vector<std::string> m_customLevels;

	std::unique_ptr<LevelEditorGui> m_gui;

	void toggleMode();
	void resetPlayer();

	void drawPlayer(sf::RenderWindow& window);
	void drawWalls(sf::RenderWindow& window);
	void drawSprites(sf::RenderWindow& window);

	void handleInputField(const sf::Event& event);
	void handleMenuCallbacks(const sf::Event& event, Game & game);
};
