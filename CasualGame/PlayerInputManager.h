#pragma once

#include "Game.h"
#include "Player.h"

class PlayerInputManager
{
public:
	PlayerInputManager();
	~PlayerInputManager();

	void setWindowWidth(int windowWidth) { m_windowWidth = windowWidth; }
	
	void handleInput(const sf::Event& event, const sf::Vector2f mousePosition);
	void updatePlayerMovement(const double fts, std::shared_ptr<Player> m_player, const std::vector<std::vector<int> >& m_levelRef);
	
	bool isShooting() const { return m_shooting; };
	bool isMoving() const { return m_forward || m_backward || m_left || m_right; }

private:

	bool m_forward = false;
	bool m_backward = false;
	bool m_left = false;
	bool m_right = false;
	bool m_shooting = false;
			
	int m_windowWidth = g_defaultWidth;

	double m_shotTime = -1.0;
	double m_gunShotDelay = -1.0;

	float m_mouseDeltaFromCenter;

	void handleMouselook(const sf::Vector2f mouseMovePos);
	void handleShot();
	void calculateShotTime(double fts);

};

