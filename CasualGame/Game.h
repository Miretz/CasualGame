#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "GameState.h"
#include "MainMenuState.h"

struct GameConfig {
	static constexpr auto windowWidth = 1024;
	static constexpr auto windowHeight = 768;
	static constexpr auto gameTitle = "Casual Game by Miretz";
	static constexpr auto ftStep = 1.f;
	static constexpr auto ftSlice = 1.f;
};

class Game
{
public:
	Game();
	virtual ~Game() = default;

	enum class GameStateName {
		MAINMENU,
		PLAY,
		QUIT
	};

	void run();
	void changeState(GameStateName newState);
	const bool isRunning() const { return m_running; };

private:
	
	float m_lastFt = 0.f;
	float m_currentSlice = 0.f;
	bool m_running = true;
	int m_fpsShowTimer = 0;

	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<sf::Clock> m_clock;
	std::unique_ptr<GameState> m_currentState;

	void checkInput();
	void update();
	void draw();
	void updateTimers();
	
};

