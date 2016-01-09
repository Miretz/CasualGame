#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <chrono>

#include "GameState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "LevelEditorState.h"

typedef float FrameTime;

static constexpr auto gameTitle = "Casual Game by Miretz";
static constexpr auto ftStep = 1.f;
static constexpr auto ftSlice = 1.f;

static constexpr auto levelFile = "resources/levels/level1.txt";
static constexpr auto levelSpriteFile = "resources/levels/level1_sprites.txt";

class Game
{
public:
	Game();
	virtual ~Game() = default;

	enum class GameStateName {
		MAINMENU,
		PLAY,
		LEVEL_EDITOR,
		SWITCH_FULLSCREEN,
		QUIT
	};

	void run();
	void changeState(GameStateName newState);
	const bool isRunning() const { return m_running; };
	void switchFullscreen();

private:
	
	float m_lastFt = 0.f;
	float m_currentSlice = 0.f;
	bool m_running = true;
	int m_fpsShowTimer = 0;

	bool m_fullscreen = false;

	std::unique_ptr<sf::RenderWindow> m_window;
	std::unique_ptr<sf::Clock> m_clock;
	std::unique_ptr<GameState> m_currentState;

	void checkInput();
	void update();
	void draw();
	void updateTimers();
	
};

