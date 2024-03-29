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
		const double oldDirX = m_player->direction.x;
		m_player->direction.x = m_player->direction.x * std::cos(rotSpeed) - m_player->direction.y * std::sin(rotSpeed);
		m_player->direction.y = oldDirX * std::sin(rotSpeed) + m_player->direction.y * std::cos(rotSpeed);
		const double oldPlaneX = m_player->plane.x;
		m_player->plane.x = m_player->plane.x * std::cos(rotSpeed) - m_player->plane.y * std::sin(rotSpeed);
		m_player->plane.y = oldPlaneX * std::sin(rotSpeed) + m_player->plane.y * std::cos(rotSpeed);
	}
	if (m_right)
	{
		//both camera direction and camera plane must be rotated
		const double oldDirX = m_player->direction.x;
		m_player->direction.x = m_player->direction.x * std::cos(-rotSpeed) - m_player->direction.y * std::sin(-rotSpeed);
		m_player->direction.y = oldDirX * std::sin(-rotSpeed) + m_player->direction.y * std::cos(-rotSpeed);
		const double oldPlaneX = m_player->plane.x;
		m_player->plane.x = m_player->plane.x * std::cos(-rotSpeed) - m_player->plane.y * std::sin(-rotSpeed);
		m_player->plane.y = oldPlaneX * sin(-rotSpeed) + m_player->plane.y * std::cos(-rotSpeed);
	}

	double currentX = m_player->position.x;
	double currentY = m_player->position.y;
	const double dirX = m_player->direction.x;
	const double dirY = m_player->direction.y;

	if (m_forward)
	{
		const double nextX = currentX + dirX * moveSpeed;
		const double nextY = currentY + dirY * moveSpeed;
		
		if (m_levelRef[static_cast<size_t>(nextX)][static_cast<size_t>(currentY)] == 0) {
			currentX = nextX;
		}
		if (m_levelRef[static_cast<size_t>(currentX)][static_cast<size_t>(nextY)] == 0) {
			currentY = nextY;
		}
	}
	if (m_backward)
	{
		const double nextX = currentX - dirX * moveSpeed;
		const double nextY = currentY - dirY * moveSpeed;

		if (m_levelRef[static_cast<size_t>(nextX)][static_cast<size_t>(currentY)] == 0) {
			currentX = nextX;
		}
		if (m_levelRef[static_cast<size_t>(currentX)][static_cast<size_t>(nextY)] == 0) {
			currentY = nextY;
		}
	}
	if (m_stepLeft)
	{
		// rotate vector in 90 ccw use Vector(-y, x)
		const double nextX = currentX - dirY * moveSpeed;
		const double nextY = currentY + dirX * moveSpeed;

		if (m_levelRef[static_cast<size_t>(nextX)][static_cast<size_t>(currentY)] == 0) {
			currentX = nextX;
		}
		if (m_levelRef[static_cast<size_t>(currentX)][static_cast<size_t>(nextY)] == 0) {
			currentY = nextY;
		}
	}
	if (m_stepRight)
	{
		// rotate vector in 90 cw use Vector(y, -x)
		const double nextX = currentX + dirY * moveSpeed;
		const double nextY = currentY - dirX * moveSpeed;

		if (m_levelRef[static_cast<size_t>(nextX)][static_cast<size_t>(currentY)] == 0) {
			currentX = nextX;
		}
		if (m_levelRef[static_cast<size_t>(currentX)][static_cast<size_t>(nextY)] == 0) {
			currentY = nextY;
		}
	}

	if (m_forward || m_backward || m_stepLeft || m_stepRight)
	{
		m_player->position.x = currentX;
		m_player->position.y = currentY;
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