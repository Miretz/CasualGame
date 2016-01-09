#include "LevelEditorState.h"

LevelEditorState::LevelEditorState(const int w, const int h, std::shared_ptr<LevelReaderWriter> levelReader) : m_windowWidth(w), m_windowHeight(h), 
m_levelReader(move(levelReader))
{
	m_scale_x = w / m_levelReader->getLevel().size();
	m_scale_y = h / m_levelReader->getLevel().size();
}


LevelEditorState::~LevelEditorState()
{
}

void LevelEditorState::update(const float ft)
{
}

void LevelEditorState::draw(sf::RenderWindow & window)
{
	
	window.clear();
	
	const size_t levelSize = m_levelReader->getLevel().size();

	//Render minimap
	sf::RectangleShape minimapBg(sf::Vector2f(m_windowWidth, m_windowHeight));
	minimapBg.setPosition(0, 0);
	minimapBg.setFillColor(sf::Color(150, 150, 150, 255));
	window.draw(minimapBg);
	for (size_t x = 0; x < levelSize; x++)
	{
		for (size_t y = 0; y < levelSize; y++)
		{
			//draw walls
			if (m_levelReader->getLevel()[x][y] > 0 && m_levelReader->getLevel()[x][y] < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(m_scale_x, m_scale_y));
				wall.setPosition(x * m_scale_x, y * m_scale_y);
				wall.setFillColor(sf::Color(0, 0, 0, 255));
				window.draw(wall);
			}
		}
	}
	// Render entities on minimap
	const size_t spritesSize = m_levelReader->getSprites().size();
	for (int i = 0; i < spritesSize; i++)
	{
		sf::CircleShape object(m_scale_x / 4.0f);
		object.setPosition(m_levelReader->getSprites()[i].x * m_scale_x, m_levelReader->getSprites()[i].y * m_scale_y);
		object.setOrigin(m_scale_x / 2.0f, m_scale_y / 2.0f);
		object.setFillColor(sf::Color(0, 0, 255, 255));
		window.draw(object);
	}

	window.display();

}

void LevelEditorState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			game.changeState(Game::GameStateName::MAINMENU);
		}
	}
}


