#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Game;
struct Player;

class PlayerInputManager
{
public:
	void handleInput(const sf::Event& event);
	void updatePlayerMovement(const double fts, std::shared_ptr<Player> m_player, const std::vector<std::vector<int> >& m_levelRef);

	constexpr bool isShooting() const { return m_shooting; };
	constexpr bool isMoving() const { return m_forward || m_backward || m_left || m_right || m_stepLeft || m_stepRight; }

private:

	bool m_forward = false;
	bool m_backward = false;
	bool m_left = false;
	bool m_right = false;

	bool m_stepLeft = false;
	bool m_stepRight = false;

	bool m_shooting = false;

	double m_shotTime = -1.0;
	double m_gunShotDelay = -1.0;

	void handleShot();
	void calculateShotTime(const double fts);

};
