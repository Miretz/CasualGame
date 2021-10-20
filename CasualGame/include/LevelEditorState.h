#pragma once

#include <memory>

#include "GameState.h"

struct Player;
class Game;
class LevelReaderWriter;
class LevelEditorGui;

class LevelEditorState : public GameState
{
public:
	LevelEditorState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);

	void update(const float ft, Game& game) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousePosition, Game& game) override;

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

	sf::Text m_statusBar;
	sf::Vector2f m_mousePos;
	std::string m_customLevelName = "<enter filename>";
	std::vector<std::string> m_customLevels;

	std::unique_ptr<LevelEditorGui> m_gui;

	void toggleMode();
	void resetPlayer() const;

	void drawPlayer(sf::RenderWindow& window) const;
	void drawWalls(sf::RenderWindow& window) const;
	void drawSprites(sf::RenderWindow& window) const;

	void handleInputField(const sf::Event& event);
	bool handleMenuCallbacks(Game& game);
	void handleKeyEvent(const sf::Event& event, Game& game, bool mouseInEditor);
	void handleMouseEditEntities(const sf::Event& event);
	void handleMouseEditWalls(const sf::Event& event) const;
};
