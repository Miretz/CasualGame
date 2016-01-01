#include "PlayState.h"

#include <stdlib.h>
#include <math.h>

RandomGenerator PlayState::gen = RandomGenerator();

const int PlayState::m_level[24][24] =
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

PlayState::PlayState() : m_pos(22.0f, 12.0f), m_dir(-1.0f, 0.0f), m_plane(0.0f, 0.66f)
{
	//colors for background
	for (int i = 0; i < 4; ++i)
	{
		m_bgColors.emplace_back(0, 0, 0, 150);
	}
}


PlayState::~PlayState()
{
}

void PlayState::update(float ft)
{

	const int colorMax = 100;

	//slight bg color update
	for (size_t i = 0, ilen = m_bgColors.size(); i < ilen; ++i)
	{

		if (gen.randomInt(0, 100) < 10)
		{
			m_bgColors[i].r += gen.randomInt(-2, 2);
			m_bgColors[i].g += gen.randomInt(-2, 2);
			m_bgColors[i].b += gen.randomInt(-2, 2);
		}

		//limit color max
		if (m_bgColors[i].r > colorMax) m_bgColors[i].r = colorMax;
		if (m_bgColors[i].g > colorMax) m_bgColors[i].g = colorMax;
		if (m_bgColors[i].b > colorMax) m_bgColors[i].b = colorMax;
		if (m_bgColors[i].r < 0) m_bgColors[i].r = 0;
		if (m_bgColors[i].g < 0) m_bgColors[i].g = 0;
		if (m_bgColors[i].b < 0) m_bgColors[i].b = 0;
	}

	//2d raycaster update
	std::vector<LineData>().swap(m_lines);
	for (int x = 0; x < GameConfig::windowWidth; x+=5)
	{
		
		float cameraX = 2 * x / static_cast<float>(GameConfig::windowWidth) - 1;
		sf::Vector2f rayPos(m_pos.x, m_pos.y);
		sf::Vector2f rayDir(m_dir.x + m_plane.x * cameraX, m_dir.y + m_plane.y * cameraX);
		sf::Vector2i map(static_cast<int>(rayPos.x), static_cast<int>(rayPos.y));
		
		sf::Vector2f deltaDist;
		deltaDist.x = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
		deltaDist.y = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));
		
		sf::Vector2f sideDist;
		sf::Vector2i stepDir;

		bool hit = false;
		int side;
		
		if (rayDir.x < 0.0f)
		{
			stepDir.x = -1;
			sideDist.x = (rayPos.x - map.x) * deltaDist.x;
		}
		else
		{
			stepDir.x = 1;
			sideDist.x = (map.x + 1.0f - rayPos.x) * deltaDist.x;
		}

		if (rayDir.y < 0.0f)
		{
			stepDir.y = -1;
			sideDist.y = (rayPos.y - map.y) * deltaDist.y;
		}
		else
		{
			stepDir.y = 1;
			sideDist.y = (map.y + 1.0f - rayPos.y) * deltaDist.y;
		}

		while (!hit)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				map.x += stepDir.x;
				side = 0;
			}
			else
			{
				sideDist.y += deltaDist.y;
				map.y += stepDir.y;
				side = 1;
			}
			if (m_level[map.x][map.y] > 0) hit = true;
		}

		double perpWallDist;
		if (side == 0)
			perpWallDist = fabs((map.x - rayPos.x + (1 - stepDir.x) / 2) / rayDir.x);
		else
			perpWallDist = fabs((map.y - rayPos.y + (1 - stepDir.y) / 2) / rayDir.y);

		int lineHeight = abs(static_cast<int>(GameConfig::windowHeight / perpWallDist));

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + GameConfig::windowHeight / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + GameConfig::windowHeight / 2;
		if (drawEnd >= GameConfig::windowHeight) drawEnd = GameConfig::windowHeight - 1;

		//choose wall color
		sf::Color color;
		switch (m_level[map.x][map.y])
		{
			case 1:  color = sf::Color::White;  break; //red
			case 2:  color = sf::Color::Yellow;  break; //green
			case 3:  color = sf::Color::Green;   break; //blue
			case 4:  color = sf::Color::Cyan;  break; //white
			default: color = sf::Color::Yellow; break; //yellow
		}

		//give x and y sides different brightness
		if (side == 1) {
			color = sf::Color(color.r / 2, color.g / 2, color.b / 2);
		}

		LineData line;
		line.x = x;
		line.drawStart = drawStart;
		line.drawEnd = drawEnd;
		line.color = color ;
		m_lines.push_back(line);

	}

	// inputs
	m_moveSpeed = ft/1000 * 5.0; //the constant value is in squares/second
	m_rotSpeed = ft/1000 * 3.0; //the constant value is in radians/second
	
	// process movement
	if (m_movement[0])
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = m_dir.x;
		m_dir.x = m_dir.x * cos(m_rotSpeed) - m_dir.y * sin(m_rotSpeed);
		m_dir.y = oldDirX * sin(m_rotSpeed) + m_dir.y * cos(m_rotSpeed);
		double oldPlaneX = m_plane.x;
		m_plane.x = m_plane.x * cos(m_rotSpeed) - m_plane.y * sin(m_rotSpeed);
		m_plane.y = oldPlaneX * sin(m_rotSpeed) + m_plane.y * cos(m_rotSpeed);
	}
	else if (m_movement[1]) 
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = m_dir.x;
		m_dir.x = m_dir.x * cos(-m_rotSpeed) - m_dir.y * sin(-m_rotSpeed);
		m_dir.y = oldDirX * sin(-m_rotSpeed) + m_dir.y * cos(-m_rotSpeed);
		double oldPlaneX = m_plane.x;
		m_plane.x = m_plane.x * cos(-m_rotSpeed) - m_plane.y * sin(-m_rotSpeed);
		m_plane.y = oldPlaneX * sin(-m_rotSpeed) + m_plane.y * cos(-m_rotSpeed);
	}
	else if (m_movement[2])
	{
		if (m_level[int(m_pos.x + m_dir.x * m_moveSpeed)][int(m_pos.y)] == false) m_pos.x += m_dir.x * m_moveSpeed;
		if (m_level[int(m_pos.x)][int(m_pos.y + m_dir.y * m_moveSpeed)] == false) m_pos.y += m_dir.y * m_moveSpeed;
	}
	else if (m_movement[3])
	{
		if (m_level[int(m_pos.x - m_dir.x * m_moveSpeed)][int(m_pos.y)] == false) m_pos.x -= m_dir.x * m_moveSpeed;
		if (m_level[int(m_pos.x)][int(m_pos.y - m_dir.y * m_moveSpeed)] == false) m_pos.y -= m_dir.y * m_moveSpeed;
	}

}

void PlayState::draw(sf::RenderWindow & window)
{
	window.clear();

	//bg rectangle
	sf::Vertex bgRect[] =
	{
		sf::Vertex(sf::Vector2f(0.f,0.f), m_bgColors[0]),
		sf::Vertex(sf::Vector2f(0.f,GameConfig::windowHeight), m_bgColors[1]),
		sf::Vertex(sf::Vector2f(GameConfig::windowWidth, GameConfig::windowHeight), m_bgColors[2]),
		sf::Vertex(sf::Vector2f(GameConfig::windowWidth, 0.0f), m_bgColors[3])
	};
	window.draw(bgRect, 4, sf::Quads);
	
	std::vector<sf::Vertex> topVertices;
	std::vector<sf::Vertex> bottomVertices;

	for (auto& lineData : m_lines)
	{
		
		sf::Vertex top(sf::Vector2f(lineData.x, lineData.drawStart), lineData.color);
		sf::Vertex bottom(sf::Vector2f(lineData.x, lineData.drawEnd), lineData.color);

		sf::Vertex line[2] = { top, bottom};
		
		topVertices.push_back(top);
		bottomVertices.push_back(bottom);

		window.draw(line, 2, sf::Lines);

	}

	window.draw(&topVertices[0], topVertices.size(), sf::Lines);
	window.draw(&bottomVertices[0], bottomVertices.size(), sf::Lines);
	
	window.display();

	//clean
	std::vector<sf::Vertex>().swap(topVertices);
	std::vector<sf::Vertex>().swap(bottomVertices);

}

void PlayState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game)
{

	m_mousePos = mousepPosition;

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed) 
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			game.changeState(Game::GameStateName::MAINMENU);
		}

		// handle controls
		if (event.key.code == sf::Keyboard::Left)
		{
			m_movement[0] = true;
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_movement[1] = true;
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			m_movement[2] = true;
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			m_movement[3] = true;
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		// handle controls
		if (event.key.code == sf::Keyboard::Left)
		{
			m_movement[0] = false;
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_movement[1] = false;
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			m_movement[2] = false;
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			m_movement[3] = false;
		}
	}
	
}
