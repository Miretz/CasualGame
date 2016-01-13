#include "Game.h"

Game::Game()
{
		
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1024, 768), gameTitle, sf::Style::Close);
	m_currentState = std::make_unique<MainMenuState>(1024, 768);
	m_clock = std::make_unique<sf::Clock>();
	
	m_levelReader = std::make_shared<LevelReaderWriter>();
	m_player = std::make_shared<Player>();

	//m_window->setFramerateLimit(500);
	//m_window->setVerticalSyncEnabled(true);
}

void Game::run()
{
	//Main Loop
	while (m_running &&  m_window->isOpen())
	{
		checkInput();
		update();
		draw();
		updateTimers();
	}
	m_window->close();
}

void Game::checkInput()
{
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}
		else
		{
			sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(*m_window);
			m_currentState->handleInput(event, mousePosition, *this);
		}
	}
}

void Game::update() 
{
	m_currentSlice += m_lastFt;

	for (; m_currentSlice >= 1.0f; m_currentSlice -= 1.0f)
	{
		m_currentState->update(1.0f);
	}
}

void Game::draw()
{
	m_currentState->draw(*m_window);
}

void Game::updateTimers()
{
	float ft = static_cast<float>(m_clock->restart().asMilliseconds());
	m_lastFt = ft;
	
	if (m_fpsShowTimer == 0)
	{
		m_fpsShowTimer = 10;
		float ftSeconds = ft / 1000.f;
		if (ftSeconds > 0.f)
		{
			int fps = static_cast<int>(1.f / ftSeconds);
			m_window->setTitle("FPS: " + std::to_string(fps) + " " + gameTitle);
		}
	}
	m_fpsShowTimer--;
}

void Game::changeState(GameStateName newState)
{
	switch (newState)
	{
	case GameStateName::MAINMENU:
		m_currentState = std::move(std::unique_ptr<MainMenuState>(new MainMenuState(m_window->getSize().x, m_window->getSize().y)));
		break;
	case GameStateName::PLAY:
		m_currentState = std::move(std::unique_ptr<PlayState>(new PlayState(m_window->getSize().x, m_window->getSize().y, m_player, m_levelReader)));
		break;
	case GameStateName::LEVEL_EDITOR:
		m_currentState = std::move(std::unique_ptr<LevelEditorState>(new LevelEditorState(m_window->getSize().x, m_window->getSize().y, m_player, m_levelReader)));
		break;
	case GameStateName::SWITCH_FULLSCREEN:
		switchFullscreen();
		break;
	case GameStateName::QUIT:
		m_running = false;
		break;
	default:
		m_currentState = std::move(std::unique_ptr<MainMenuState>(new MainMenuState(m_window->getSize().x, m_window->getSize().y)));
		break;
	}
}

void Game::switchFullscreen()
{
	if (!m_fullscreen)
	{
		m_window->close();
		m_window->create(sf::VideoMode::getDesktopMode(), gameTitle, sf::Style::Fullscreen);
		changeState(GameStateName::MAINMENU);
		m_fullscreen = true;
	}
	else
	{
		m_window->close();
		m_window->create(sf::VideoMode(1024, 768), gameTitle, sf::Style::Close);
		changeState(GameStateName::MAINMENU);
		m_fullscreen = false;
	}
}
