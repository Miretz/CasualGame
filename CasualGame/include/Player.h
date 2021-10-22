#pragma once

struct Player
{
	sf::Vector2<double> position{22.0, 11.5};
	sf::Vector2<double> direction{ -1.0, 0.0 };
	sf::Vector2<double> plane{ 0.0, 0.66 };
	int m_health = 100;
};

