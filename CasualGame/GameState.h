#pragma once

#include <SFML/Graphics.hpp>

class Game;

class GameState
{
public:

	virtual void update(float ft) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition, Game& game) = 0;

	virtual ~GameState() {};

};

