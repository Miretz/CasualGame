#pragma once

#include "GameState.h"
#include "Game.h"
#include "Sprite.h"

class LevelEditorState : public GameState
{
public:
	LevelEditorState(const int w, const int h);
	~LevelEditorState();

	void update(const float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	const int m_windowWidth;
	const int m_windowHeight;
	
	float m_scale_x;
	float m_scale_y;

	std::vector<std::vector<int> >m_level;
	std::vector<Sprite> m_sprites;

	void loadLevel();
	void loadLevelSprites();

};
