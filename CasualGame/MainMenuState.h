#pragma once

#include "GameState.h"
#include "Game.h"

class MainMenuState : public GameState
{
public:
	MainMenuState();
	~MainMenuState();

	void update(float ft) override;
	void draw(sf::RenderWindow& window) override;
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) override;

private:

	sf::Font m_font;
	std::vector<sf::Text> m_menuItems;
	sf::Text m_titleText;
	
	sf::RenderTexture m_myRenderTexture;
	sf::Sprite m_spriteWorld;
	sf::Shader m_shader;

	sf::Vector2f m_mousePos;

	float m_padding = 20.0f;
	int m_mouseOverIndex = 0;

};

