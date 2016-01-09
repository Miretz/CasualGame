#include "LevelEditorState.h"

LevelEditorState::LevelEditorState(const int w, const int h) : m_windowWidth(w), m_windowHeight(h)
{
	// load the level
	loadLevel();
	loadLevelSprites();

	m_scale_x = w / m_level.size();
	m_scale_y = h / m_level.size();

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
	
	//Render minimap
	sf::RectangleShape minimapBg(sf::Vector2f(m_windowWidth, m_windowHeight));
	minimapBg.setPosition(0, 0);
	minimapBg.setFillColor(sf::Color(150, 150, 150, 255));
	window.draw(minimapBg);
	for (size_t x = 0; x < m_level.size(); x++)
	{
		for (size_t y = 0; y < m_level.size(); y++)
		{
			//draw walls
			if (m_level[x][y] > 0 && m_level[x][y] < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(m_scale_x, m_scale_y));
				wall.setPosition(x * m_scale_x, y * m_scale_y);
				wall.setFillColor(sf::Color(0, 0, 0, 255));
				window.draw(wall);
			}
		}
	}
	// Render entities on minimap
	for (int i = 0; i < m_sprites.size(); i++)
	{
		sf::CircleShape object(m_scale_x / 4.0f);
		object.setPosition(m_sprites[i].x * m_scale_x, m_sprites[i].y * m_scale_y);
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

void LevelEditorState::loadLevel()
{
	std::ifstream file(levelFile);

	std::string line;
	while (std::getline(file, line))
	{
		if (!file.good())
			break;

		std::stringstream iss(line);
		std::string val;

		std::vector<int> rowVec;

		while (std::getline(iss, val, ','))
		{
			if (!iss.good())
				break;

			int converted = 0;
			std::stringstream convertor(val);
			convertor >> converted;
			rowVec.push_back(converted);
		}

		m_level.push_back(rowVec);
	}
}

void LevelEditorState::loadLevelSprites()
{
	int row = 0;
	std::ifstream file(levelSpriteFile);
	std::string line;
	while (std::getline(file, line))
	{
		if (!file.good())
			break;

		std::stringstream iss(line);

		Sprite spr;
		for (int col = 0; col < 3; col++)
		{
			std::string val;
			std::getline(iss, val, ',');
			if (!iss.good())
				break;

			std::stringstream convertor(val);

			switch (col)
			{
			case 0: convertor >> spr.x; break;
			case 1: convertor >> spr.y; break;
			case 2: convertor >> spr.texture; break;
			default: break;
			}
		}
		m_sprites.push_back(spr);
	}
}
