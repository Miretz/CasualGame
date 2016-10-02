#include "PlayerInputManager.h"

PlayerInputManager::PlayerInputManager() {}
PlayerInputManager::~PlayerInputManager() {}

void PlayerInputManager::handleInput(const sf::Event & event, const sf::Vector2f mousePosition)
{

	if (event.type == sf::Event::MouseButtonPressed)
	{
		handleShot();
	}

	handleMouselook(mousePosition);

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed)
	{
		// handle controls
		if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A))
		{
			//TODO: sidestep
			//m_left = true;
		}
		else if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D))
		{
			//TODO: sidestep
			//m_right = true;
		}
		else if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W))
		{
			m_forward = true;
		}
		else if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S))
		{
			m_backward = true;
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		// handle controls
		if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A))
		{
			//TODO: sidestep
			//m_left = false;
		}
		if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D))
		{
			//TODO: sidestep
			//m_right = false;
		}
		if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W))
		{
			m_forward = false;
		}
		if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S))
		{
			m_backward = false;
		}
		if ((event.key.code == sf::Keyboard::Space) || (event.key.code == sf::Keyboard::LControl))
		{
			handleShot();
		}
	}
}

void PlayerInputManager::updatePlayerMovement(const double fts, std::shared_ptr<Player> m_player, const std::vector<std::vector<int> >& m_levelRef)
{
	calculateShotTime(fts);

	//update position
	if (isMoving())
	{
		// convert ms to seconds
		double moveSpeed = fts * 5.0; //the constant value is in squares/second
		double rotSpeed = fts * fabs(m_mouseDeltaFromCenter / m_windowWidth) * 5.0f; //the constant value is in radians/second

		if (m_left)
		{
			//both camera direction and camera plane must be rotated
			double oldDirX = m_player->m_dirX;
			m_player->m_dirX = m_player->m_dirX * cos(rotSpeed) - m_player->m_dirY * sin(rotSpeed);
			m_player->m_dirY = oldDirX * sin(rotSpeed) + m_player->m_dirY * cos(rotSpeed);
			double oldPlaneX = m_player->m_planeX;
			m_player->m_planeX = m_player->m_planeX * cos(rotSpeed) - m_player->m_planeY * sin(rotSpeed);
			m_player->m_planeY = oldPlaneX * sin(rotSpeed) + m_player->m_planeY * cos(rotSpeed);
		}
		if (m_right)
		{
			//both camera direction and camera plane must be rotated
			double oldDirX = m_player->m_dirX;
			m_player->m_dirX = m_player->m_dirX * cos(-rotSpeed) - m_player->m_dirY * sin(-rotSpeed);
			m_player->m_dirY = oldDirX * sin(-rotSpeed) + m_player->m_dirY * cos(-rotSpeed);
			double oldPlaneX = m_player->m_planeX;
			m_player->m_planeX = m_player->m_planeX * cos(-rotSpeed) - m_player->m_planeY * sin(-rotSpeed);
			m_player->m_planeY = oldPlaneX * sin(-rotSpeed) + m_player->m_planeY * cos(-rotSpeed);
		}
		if (m_forward)
		{
			if (m_levelRef[int(m_player->m_posX + m_player->m_dirX * moveSpeed)][int(m_player->m_posY)] == false) m_player->m_posX += m_player->m_dirX * moveSpeed;
			if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY + m_player->m_dirY * moveSpeed)] == false) m_player->m_posY += m_player->m_dirY * moveSpeed;
		}
		if (m_backward)
		{
			if (m_levelRef[int(m_player->m_posX - m_player->m_dirX * moveSpeed)][int(m_player->m_posY)] == false) m_player->m_posX -= m_player->m_dirX * moveSpeed;
			if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY - m_player->m_dirY * moveSpeed)] == false) m_player->m_posY -= m_player->m_dirY * moveSpeed;
		}
	}
}

void PlayerInputManager::calculateShotTime(double fts)
{
	if (m_shotTime < 0.0)
	{
		m_shooting = false;
	}
	else
	{
		m_shotTime -= fts;
	}
	if (m_gunShotDelay > 0.0)
	{
		m_gunShotDelay -= fts;
	}
}

void PlayerInputManager::handleShot()
{
	if (m_shotTime > 0.0 || m_gunShotDelay > 0.0)
	{
		m_shooting = false;
		return;
	}
	m_shotTime = g_gunShotTime;
	m_gunShotDelay = g_gunShotDelayTime;
	m_shooting = true;
}


void PlayerInputManager::handleMouselook(const sf::Vector2f mouseMovePos)
{
	float centerX = m_windowWidth / 2.0f;
	m_mouseDeltaFromCenter = centerX - mouseMovePos.x;

	if (m_mouseDeltaFromCenter > 10.0f)
	{
		m_right = false;
		m_left = true;
	}
	else if (m_mouseDeltaFromCenter < -10.0f)
	{
		m_left = false;
		m_right = true;
	}
	else
	{
		m_left = false;
		m_right = false;
	}
}