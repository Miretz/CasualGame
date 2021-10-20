#pragma once

#include <SFML/Graphics.hpp>

class Game;

// Game states
enum class GameStateName
{
	MAINMENU,
	PLAY,
	RESTART,
	LEVEL_EDITOR,
	SWITCH_FULLSCREEN,
	QUIT
};

class GameState
{
public:
	GameState() = default;

	virtual void update(const float ft) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void handleInput(const sf::Event& event, const sf::Vector2f& mousePosition, Game& game) = 0;

	virtual ~GameState() = default;

	GameState& operator=(const GameState&) = delete;
	GameState(const GameState&) = delete;
};

