#include "PlayerInputManager.h"

#include "Game.h"
#include "Player.h"
#include "Config.h"

#include <cmath>


void PlayerInputManager::handleInput(const sf::Event& event)
{

	if (event.type == sf::Event::MouseButtonPressed)
	{
		handleShot();
	}

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Left:
		case sf::Keyboard::A:
			m_left = true; break;
		case sf::Keyboard::Right:
		case sf::Keyboard::D:
			m_right = true; break;
		case sf::Keyboard::Up:
		case sf::Keyboard::W:
			m_forward = true; break;
		case sf::Keyboard::Down:
		case sf::Keyboard::S:
			m_backward = true; break;
		default: break;
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		// handle controls
		if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A))
		{
			m_left = false;
		}
		if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D))
		{
			m_right = false;
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
	if (!isMoving())
	{
		return;
	}

	// convert ms to seconds
	const double moveSpeed = fts * g_moveSpeed; //the constant value is in squares/second
	const double rotSpeed = fts * g_lookSpeed;

	if (m_left)
	{
		//both camera direction and camera plane must be rotated
		const double oldDirX = m_player->m_dirX;
		m_player->m_dirX = m_player->m_dirX * std::cos(rotSpeed) - m_player->m_dirY * std::sin(rotSpeed);
		m_player->m_dirY = oldDirX * std::sin(rotSpeed) + m_player->m_dirY * std::cos(rotSpeed);
		const double oldPlaneX = m_player->m_planeX;
		m_player->m_planeX = m_player->m_planeX * std::cos(rotSpeed) - m_player->m_planeY * std::sin(rotSpeed);
		m_player->m_planeY = oldPlaneX * std::sin(rotSpeed) + m_player->m_planeY * std::cos(rotSpeed);
	}
	if (m_right)
	{
		//both camera direction and camera plane must be rotated
		const double oldDirX = m_player->m_dirX;
		m_player->m_dirX = m_player->m_dirX * std::cos(-rotSpeed) - m_player->m_dirY * std::sin(-rotSpeed);
		m_player->m_dirY = oldDirX * std::sin(-rotSpeed) + m_player->m_dirY * std::cos(-rotSpeed);
		const double oldPlaneX = m_player->m_planeX;
		m_player->m_planeX = m_player->m_planeX * std::cos(-rotSpeed) - m_player->m_planeY * std::sin(-rotSpeed);
		m_player->m_planeY = oldPlaneX * sin(-rotSpeed) + m_player->m_planeY * std::cos(-rotSpeed);
	}
	if (m_forward)
	{
		if (m_levelRef[int(m_player->m_posX + m_player->m_dirX * moveSpeed)][int(m_player->m_posY)] == 0)
			m_player->m_posX += m_player->m_dirX * moveSpeed;
		if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY + m_player->m_dirY * moveSpeed)] == 0)
			m_player->m_posY += m_player->m_dirY * moveSpeed;
	}
	if (m_backward)
	{
		if (m_levelRef[int(m_player->m_posX - m_player->m_dirX * moveSpeed)][int(m_player->m_posY)] == 0)
			m_player->m_posX -= m_player->m_dirX * moveSpeed;
		if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY - m_player->m_dirY * moveSpeed)] == 0)
			m_player->m_posY -= m_player->m_dirY * moveSpeed;
	}
	if (m_stepLeft)
	{
		// rotate vector in 90 ccw use Vector(-y, x)
		const auto dirX = -m_player->m_dirY;
		const auto dirY = m_player->m_dirX;

		if (m_levelRef[int(m_player->m_posX + dirX * moveSpeed)][int(m_player->m_posY)] == 0)
			m_player->m_posX += dirX * moveSpeed;
		if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY + dirY * moveSpeed)] == 0)
			m_player->m_posY += dirY * moveSpeed;
	}
	if (m_stepRight)
	{
		// rotate vector in 90 cw use Vector(y, -x)
		const auto dirX = m_player->m_dirY;
		const auto dirY = -m_player->m_dirX;

		if (m_levelRef[int(m_player->m_posX + dirX * moveSpeed)][int(m_player->m_posY)] == 0)
			m_player->m_posX += dirX * moveSpeed;
		if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY + dirY * moveSpeed)] == 0)
			m_player->m_posY += dirY * moveSpeed;
	}
}

void PlayerInputManager::calculateShotTime(const double fts)
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