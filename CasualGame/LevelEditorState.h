#pragma once

#include "GameState.h"
#include "Game.h"
#include "Sprite.h"
#include "LevelReaderWriter.h"

class LevelEditorState : public GameState
{
public:
	LevelEditorState(const int w, const int h, std::shared_ptr<LevelReaderWriter> levelReader);
	~LevelEditorState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	std::shared_ptr<LevelReaderWriter> m_levelReader;

	const int m_windowWidth;
	const int m_windowHeight;
	
	float m_scale_x;
	float m_scale_y;

};
