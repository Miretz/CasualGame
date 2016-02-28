#include "Game.h"

#include "MainMenuState.h"
#include "PlayState.h"
#include "LevelEditorState.h"

Game::Game() {
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(g_defaultWidth, g_defaultHeight), g_gameTitle, sf::Style::Close);
	m_currentState = std::make_unique<MainMenuState>(g_defaultWidth, g_defaultHeight);
	m_clock = std::make_unique<sf::Clock>();
	
	m_window->setFramerateLimit(500);

	m_levelReader = std::make_shared<LevelReaderWriter>();
	m_player = std::make_shared<Player>();
}

void Game::run() {
	//Main Loop
	while (m_running) {
		checkInput();
		update();
		draw();
		updateTimers();
	}
	m_window->close();
}

void Game::checkInput() {
	sf::Event event;
	while (m_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_running = false;
		}
		else  {
			auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_window));
			m_currentState->handleInput(event, mousePosition, *this);
		}
	}
}

void Game::update() {
	m_currentSlice += m_lastFt;
	for (; m_currentSlice >= 1.0f; m_currentSlice -= 1.0f) {
		m_currentState->update(1.0f);
	}
}

void Game::draw() const {
	m_currentState->draw(*m_window);
}

void Game::updateTimers() {
	auto ft = static_cast<float>(m_clock->restart().asMilliseconds());
	m_lastFt = ft;
	
	if (m_fpsShowTimer == 0) {
		m_fpsShowTimer = 2;
		auto ftSeconds = ft / 1000.f;
		if (ftSeconds > 0.f) {
			m_fps = static_cast<int>(1.f / ftSeconds);
		}
	}
	m_fpsShowTimer--;
}

void Game::changeState(GameStateName newState) {
	switch (newState) {
	case GameStateName::MAINMENU:
		m_currentState = std::move(std::make_unique<MainMenuState>(m_window->getSize().x, m_window->getSize().y));
		break;
	case GameStateName::PLAY:
		m_currentState = std::move(std::make_unique<PlayState>(m_window->getSize().x, m_window->getSize().y, m_player, m_levelReader));
		break;
	case GameStateName::RESTART:
		restart();
		break;
	case GameStateName::LEVEL_EDITOR:
		m_currentState = std::move(std::make_unique<LevelEditorState>(m_window->getSize().x, m_window->getSize().y, m_player, m_levelReader));
		break;
	case GameStateName::SWITCH_FULLSCREEN:
		switchFullscreen();
		break;
	case GameStateName::QUIT:
		m_running = false;
		break;
	default:
		m_currentState = std::move(std::make_unique<MainMenuState>(m_window->getSize().x, m_window->getSize().y));
		break;
	}
}

void Game::restart() {
	//reset player position
	m_player->m_posX = 22.0;
	m_player->m_posY = 11.5;
	m_player->m_dirX = -1.0;
	m_player->m_dirY = 0.0;
	m_player->m_planeX = 0.0;
	m_player->m_planeY = 0.66;

	//reload level
	m_levelReader->loadDefaultLevel();
	
	m_currentState = std::move(std::make_unique<PlayState>(m_window->getSize().x, m_window->getSize().y, m_player, m_levelReader));
}

void Game::switchFullscreen() {
	if (!m_fullscreen) {
		m_window->close();
		m_window->create(sf::VideoMode::getDesktopMode(), g_gameTitle, sf::Style::Fullscreen);
		changeState(GameStateName::MAINMENU);
		m_fullscreen = true;
	}
	else {
		m_window->close();
		m_window->create(sf::VideoMode(1024, 768), g_gameTitle, sf::Style::Close);
		changeState(GameStateName::MAINMENU);
		m_fullscreen = false;
	}
	m_window->setFramerateLimit(500);
}
