#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>

#include "GameState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "LevelEditorState.h"
#include "LevelReaderWriter.h"

static const auto gameTitle = "Casual Game by Miretz";
static const auto defaultWidth = 1024;
static const auto defaultHeight = 768;

class Game {
public:
	Game();
	virtual ~Game() = default;

	enum class GameStateName {
		MAINMENU,
		PLAY,
		RESTART,
		LEVEL_EDITOR,
		SWITCH_FULLSCREEN,
		QUIT
	};

	void run();
	void changeState(GameStateName newState);
	const bool isRunning() const { return m_running; };
	void switchFullscreen();
	
	const int getFps() const { return m_fps; }

private:
	
	float m_lastFt = 0.f;
	float m_currentSlice = 0.f;
	bool m_running = true;
	int m_fpsShowTimer = 0;
	bool m_fullscreen = false;

	int m_fps;

	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<sf::Clock> m_clock;
	std::unique_ptr<GameState> m_currentState;
	
	std::shared_ptr<LevelReaderWriter> m_levelReader;
	std::shared_ptr<Player> m_player;

	void checkInput();
	void update();
	void draw();
	void updateTimers();
	
};

