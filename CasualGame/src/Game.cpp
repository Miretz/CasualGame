#include "Game.h"

#include "Player.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "LevelEditorState.h"

#include "LevelReaderWriter.h"
#include "Config.h"

Game::Game()
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(g_defaultWidth, g_defaultHeight), g_gameTitle, sf::Style::Close);
	m_currentState = std::make_unique<MainMenuState>(g_defaultWidth, g_defaultHeight);
	m_clock = std::make_unique<sf::Clock>();

	m_window->setFramerateLimit(g_frameRateLimit);
	m_window->setVerticalSyncEnabled(true);
	m_levelReader = std::make_shared<LevelReaderWriter>();
	m_player = std::make_shared<Player>();
}

void Game::run()
{
	//Main Loop
	while (m_running)
	{
		update();
		draw();
		updateTimers();
		checkInput();
	}
	m_window->close();
}

void Game::checkInput()
{
	auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_window));

	sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}
		else
		{
			m_currentState->handleInput(event, mousePosition, *this);
		}
	}
}

void Game::update()
{
	m_currentSlice += m_lastFt;
	for (; m_currentSlice >= 1; m_currentSlice -= 1)
	{
		m_currentState->update(1.0f, *this);
	}
}

void Game::draw() const
{
	m_currentState->draw(*m_window);
}

void Game::updateTimers()
{
	m_lastFt = m_clock->restart().asMilliseconds();

	if (m_fpsShowTimer == 0)
	{
		m_fpsShowTimer = 4;
		auto ftSeconds = static_cast<float>(m_lastFt) / 1000.f;
		if (ftSeconds > 0.f)
		{
			m_fps = static_cast<int>(1.f / ftSeconds);
		}
	}
	m_fpsShowTimer--;
}

void Game::changeState(GameStateName newState)
{

	const auto sizeX = m_window->getSize().x;
	const auto sizeY = m_window->getSize().y;

	switch (newState)
	{
	case GameStateName::MAINMENU:
		m_window->setMouseCursorVisible(true);
		m_currentState.reset(new MainMenuState(sizeX, sizeY));
		break;
	case GameStateName::PLAY:
		m_window->setMouseCursorVisible(false);
		m_currentState.reset(new PlayState(sizeX, sizeY, m_player, m_levelReader));
		break;
	case GameStateName::RESTART:
		m_window->setMouseCursorVisible(false);
		resetLevel();
		m_currentState.reset(new PlayState(sizeX, sizeY, m_player, m_levelReader));
		break;
	case GameStateName::LEVEL_EDITOR:
		m_window->setMouseCursorVisible(true);
		m_currentState.reset(new LevelEditorState(sizeX, sizeY, m_player, m_levelReader));
		break;
	case GameStateName::SWITCH_FULLSCREEN:
		switchFullscreen();
		m_currentState.reset(new MainMenuState(m_window->getSize().x, m_window->getSize().y));
		break;
	case GameStateName::QUIT:
		m_running = false;
		break;
	default:
		m_currentState.reset(new MainMenuState(sizeX, sizeY));
		break;
	}
}

void Game::resetLevel() const
{
	//reset player position
	m_player->position = { 22.0, 11.5 };
	m_player->direction = { -1.0, 0.0 };
	m_player->plane = { 0.0, 0.66 };

	//reload level
	m_levelReader->loadDefaultLevel();

}

void Game::switchFullscreen()
{
	if (!m_fullscreen)
	{
		m_window->close();
		m_window->create(sf::VideoMode::getDesktopMode(), g_gameTitle, sf::Style::Fullscreen);
		m_fullscreen = true;
	}
	else
	{
		m_window->close();
		m_window->create(sf::VideoMode(g_defaultWidth, g_defaultHeight), g_gameTitle, sf::Style::Close);
		m_fullscreen = false;
	}
	m_window->setFramerateLimit(g_frameRateLimit);
}