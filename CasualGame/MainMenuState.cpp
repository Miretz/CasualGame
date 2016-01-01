#include "MainMenuState.h"

MainMenuState::MainMenuState()
{
	m_font.loadFromFile("resources/font/OtherF.ttf");

	// Game Title

	m_titleText.setFont(m_font);
	m_titleText.setString("Casual Game");
	m_titleText.setCharacterSize(50);
	m_titleText.setPosition(GameConfig::windowWidth / 2.0f, 200);
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2.0f, m_titleText.getGlobalBounds().height / 2.0f);
	m_titleText.setColor(sf::Color::White);

	// Menu Items

	sf::Text startGame;
	startGame.setFont(m_font);
	startGame.setString("Start");
	startGame.setCharacterSize(30);
	startGame.setPosition(GameConfig::windowWidth / 2.0f, 300);
	startGame.setOrigin(startGame.getGlobalBounds().width / 2.0f, startGame.getGlobalBounds().height / 2.0f);
	startGame.setColor(sf::Color::White);

	m_menuItems.push_back(startGame);

	sf::Text quitGame;
	quitGame.setFont(m_font);
	quitGame.setString("Quit");
	quitGame.setCharacterSize(30);
	quitGame.setPosition(GameConfig::windowWidth / 2.0f, 350);
	quitGame.setOrigin(quitGame.getGlobalBounds().width / 2.0f, quitGame.getGlobalBounds().height / 2.0f);
	quitGame.setColor(sf::Color::White);

	m_menuItems.push_back(quitGame);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(float ft)
{
}

void MainMenuState::draw(sf::RenderWindow& window)
{
	window.clear();
	window.draw(m_titleText);

	for (int i = 0; i < m_menuItems.size(); ++i)
	{
		sf::Text item = m_menuItems[i];

		sf::FloatRect boundingBox = item.getGlobalBounds();
		sf::FloatRect boundsWithPadding(boundingBox.left - m_padding / 2.0f, boundingBox.top - m_padding / 2.0f, boundingBox.width + m_padding, boundingBox.height + m_padding);

		// draw selection box
		if (boundsWithPadding.contains(m_mousePos) || m_mouseOverIndex == i)
		{
			m_mouseOverIndex = i;
			
			sf::RectangleShape selector(sf::Vector2f(boundsWithPadding.width, boundsWithPadding.height));
			selector.setPosition(boundsWithPadding.left, boundsWithPadding.top);
			selector.setFillColor(sf::Color(0, 0, 0, 0));
			selector.setOutlineThickness(2);
			selector.setOutlineColor(sf::Color(255, 255, 255, 255));
			window.draw(selector);
		}
		
		window.draw(item);
	}

	window.display();
}

void MainMenuState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game& game)
{
	
	m_mousePos = mousepPosition;
	
	//exit
	if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
	{
		game.changeState(Game::GameStateName::QUIT);
	}
	//left mouse or enter button
	else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return) ||
		(event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
	{
		if (m_mouseOverIndex == 0) {
			game.changeState(Game::GameStateName::PLAY);
		}
		if (m_mouseOverIndex == 1) {
			game.changeState(Game::GameStateName::QUIT);
		}
	}
	//enable menu selection using arrow keys
	else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
	{
		if(m_mouseOverIndex < m_menuItems.size()-1)
			m_mouseOverIndex++;
	}
	else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
	{
		if (m_mouseOverIndex > 0)
			m_mouseOverIndex--;
	}
	
}
