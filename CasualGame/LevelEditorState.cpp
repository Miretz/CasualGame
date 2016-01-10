#include "LevelEditorState.h"

LevelEditorState::LevelEditorState(const int w, const int h, std::shared_ptr<LevelReaderWriter> levelReader) : m_windowWidth(w), m_windowHeight(h), 
m_levelReader(move(levelReader))
{
	m_scale_x = float(w) / m_levelReader->getLevel().size();
	m_scale_y = float(h) / m_levelReader->getLevel().size();
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
	
	//walls
	for (size_t y = 0; y < levelSize; y++)
	{
		for (size_t x = 0; x < levelSize; x++)
		{
			int id = m_levelReader->getLevel()[y][x];
			if (id > 0 && id < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(m_scale_x - 2.0f, m_scale_y - 2.0f));
				wall.setPosition(x * m_scale_x, y * m_scale_y);
				wall.setTexture(m_levelReader->getTextureSfml(id - 1));
				wall.setOutlineThickness(2);
				wall.setOutlineColor(sf::Color(0, 0, 0));
				window.draw(wall);
			}
		}
	}
	// Render entities on minimap
	const size_t spritesSize = m_levelReader->getSprites().size();
	for (int i = 0; i < spritesSize; i++)
	{
		sf::RectangleShape object(sf::Vector2f(m_scale_x - 2.0f, m_scale_y - 2.0f));
		object.setPosition(m_levelReader->getSprites()[i].y * m_scale_x, m_levelReader->getSprites()[i].x * m_scale_y);
		object.setOrigin(m_scale_x / 2.0f, m_scale_y / 2.0f);
		object.setOutlineThickness(2);
		object.setOutlineColor(sf::Color(255, 255, 255, 128));
		object.setTexture(m_levelReader->getTextureSfml(m_levelReader->getSprites()[i].texture));
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
	else if (event.type == sf::Event::MouseButtonPressed)
	{

		int x = mousepPosition.x / m_scale_x;
		int y = mousepPosition.y / m_scale_y;

		const int value = m_levelReader->getLevel()[y][x];

		// 1 ... 8 - walls
		// 9, 10 - floor, ceiling
		// 11,12,13 - barrel, pillar, light
		// In fact you have to substract 1 for the level 
		if (value < 8 && (event.mouseButton.button == sf::Mouse::Left))
		{
			m_levelReader->changeLevelTile(y, x, value + 1);
		}
		else
		{
			const int levelSize = m_levelReader->getLevel().size();
			//Do not allow to delete the level outer walls, this breaks the raycaster
			if (x == 0 || x == levelSize-1) 
			{
				m_levelReader->changeLevelTile(y, x, 1);
			}
			else if (y == 0 || y == levelSize-1) 
			{
				m_levelReader->changeLevelTile(y, x, 1);
			} 
			else
			{
				m_levelReader->changeLevelTile(y, x, 0);
			}
		}
	}
}


