#pragma once

#include "GameState.h"
#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "LevelEditorGui.h"

class LevelEditorState : public GameState
{
public:
	LevelEditorState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader);
	~LevelEditorState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	std::shared_ptr<Player> m_player;

	bool m_editEntities = false;
	int m_entitySelected = -1;

	sf::Vector2f m_mousePos;

	std::shared_ptr<LevelReaderWriter> m_levelReader;

	std::unique_ptr<LevelEditorGui> m_gui;

	const int m_windowWidth;
	const int m_windowHeight;
	
	sf::Font m_font;
	sf::Text m_statusBar;

	float m_scale;

	void toggleMode();

	std::vector<std::string> m_customLevels;

};
