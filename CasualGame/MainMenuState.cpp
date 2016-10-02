#include "MainMenuState.h"

RandomGenerator MainMenuState::gen = RandomGenerator();

MainMenuState::MainMenuState(const int w, const int h) :
	m_windowWidth(w),
	m_windowHeight(h)
{

	m_font.loadFromFile(g_fontPath);

	// Game Title
	sf::Color textColor = sf::Color::White;

	m_titleText.setFont(m_font);
	m_titleText.setString(g_mainTxtTitle);
	m_titleText.setCharacterSize(50);
	m_titleText.setPosition(m_windowWidth / 2.0f, 200);
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2.0f, m_titleText.getGlobalBounds().height / 2.0f);
	m_titleText.setColor(textColor);

	// Menu Items
	// Start Game
	sf::Text startGame;
	startGame.setFont(m_font);
	startGame.setString(g_mainTxtStartGame);
	startGame.setCharacterSize(30);
	startGame.setPosition(m_windowWidth / 2.0f, 300);
	startGame.setOrigin(startGame.getGlobalBounds().width / 2.0f, startGame.getGlobalBounds().height / 2.0f);
	startGame.setColor(textColor);

	m_menuItems.push_back(startGame);

	// Restart Game
	sf::Text restartGame;
	restartGame.setFont(m_font);
	restartGame.setString(g_mainTxtRestartGame);
	restartGame.setCharacterSize(30);
	restartGame.setPosition(m_windowWidth / 2.0f, 350);
	restartGame.setOrigin(restartGame.getGlobalBounds().width / 2.0f, restartGame.getGlobalBounds().height / 2.0f);
	restartGame.setColor(textColor);

	m_menuItems.push_back(restartGame);

	// Level Editor 
	sf::Text levelEditor;
	levelEditor.setFont(m_font);
	levelEditor.setString(g_mainTxtLevelEditor);
	levelEditor.setCharacterSize(30);
	levelEditor.setPosition(m_windowWidth / 2.0f, 400);
	levelEditor.setOrigin(levelEditor.getGlobalBounds().width / 2.0f, levelEditor.getGlobalBounds().height / 2.0f);
	levelEditor.setColor(textColor);

	m_menuItems.push_back(levelEditor);

	//Toggle Fullscreen
	sf::Text switchFullscreen;
	switchFullscreen.setFont(m_font);
	switchFullscreen.setString(g_mainTxtToggleFullscreen);
	switchFullscreen.setCharacterSize(30);
	switchFullscreen.setPosition(m_windowWidth / 2.0f, 450);
	switchFullscreen.setOrigin(switchFullscreen.getGlobalBounds().width / 2.0f, switchFullscreen.getGlobalBounds().height / 2.0f);
	switchFullscreen.setColor(textColor);

	m_menuItems.push_back(switchFullscreen);

	//Quit Game
	sf::Text quitGame;
	quitGame.setFont(m_font);
	quitGame.setString(g_mainTxtQuit);
	quitGame.setCharacterSize(30);
	quitGame.setPosition(m_windowWidth / 2.0f, 500);
	quitGame.setOrigin(quitGame.getGlobalBounds().width / 2.0f, quitGame.getGlobalBounds().height / 2.0f);
	quitGame.setColor(textColor);

	m_menuItems.push_back(quitGame);

	//colors for background
	for (int i = 0; i < 4; ++i)
	{
		m_bgColors.emplace_back(0, 0, 0, 150);
	}

	// followers vector
	for (int i = 0; i < 200; ++i)
	{
		sf::CircleShape circle(1.5f);
		circle.setFillColor(sf::Color::White);
		circle.setPosition(gen.randomFloat(0.0f, static_cast<float>(m_windowWidth)), gen.randomFloat(0.0f, static_cast<float>(m_windowWidth)));
		m_followers.push_back(circle);
	}

}

void MainMenuState::update(const float ft)
{

	sf::Transform rotation;
	rotation.rotate(0.01f, m_windowWidth / 2.0f, m_windowHeight / 2.0f);
	for (size_t i = 0; i < m_followers.size(); ++i)
	{
		m_followers[i].setPosition(rotation.transformPoint(m_followers[i].getPosition()));
	}

	const int colorMax = 100;

	//slight bg color update
	for (size_t i = 0; i < m_bgColors.size(); ++i)
	{

		if (gen.randomInt(0, 100) < 10)
		{
			m_bgColors[i].r += gen.randomInt(-2, 2);
			m_bgColors[i].g += gen.randomInt(-2, 2);
			m_bgColors[i].b += gen.randomInt(-2, 2);
		}

		//limit color max
		if (m_bgColors[i].r > colorMax) m_bgColors[i].r = colorMax;
		if (m_bgColors[i].g > colorMax) m_bgColors[i].g = colorMax;
		if (m_bgColors[i].b > colorMax) m_bgColors[i].b = colorMax;
		if (m_bgColors[i].r <= 0) m_bgColors[i].r = 0;
		if (m_bgColors[i].g <= 0) m_bgColors[i].g = 0;
		if (m_bgColors[i].b <= 0) m_bgColors[i].b = 0;
	}

}

void MainMenuState::draw(sf::RenderWindow& window)
{
	window.clear();

	//draw followers 
	for (const auto follower : m_followers)
	{
		window.draw(follower);
	}

	//bg rectangle
	sf::Vertex bgRect[] = {
		sf::Vertex(sf::Vector2f(0.f,0.f), m_bgColors[0]),
		sf::Vertex(sf::Vector2f(0.f,static_cast<float>(m_windowHeight)), m_bgColors[1]),
		sf::Vertex(sf::Vector2f(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)), m_bgColors[2]),
		sf::Vertex(sf::Vector2f(static_cast<float>(m_windowWidth), 0.0f), m_bgColors[3])
	};
	window.draw(bgRect, 4, sf::Quads);

	window.draw(m_titleText);

	for (size_t i = 0; i < m_menuItems.size(); ++i)
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

void MainMenuState::handleInput(const sf::Event & event, const sf::Vector2f mousepPosition, Game& game)
{

	m_mousePos = mousepPosition;

	//exit
	if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
	{
		game.changeState(GameStateName::QUIT);
	}
	//left mouse or enter button
	else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return) ||
		(event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
	{
		if (m_mouseOverIndex == 0)
		{
			game.changeState(GameStateName::PLAY);
		}
		if (m_mouseOverIndex == 1)
		{
			game.changeState(GameStateName::RESTART);
		}
		if (m_mouseOverIndex == 2)
		{
			game.changeState(GameStateName::LEVEL_EDITOR);
		}
		if (m_mouseOverIndex == 3)
		{
			game.changeState(GameStateName::SWITCH_FULLSCREEN);
		}
		if (m_mouseOverIndex == 4)
		{
			game.changeState(GameStateName::QUIT);
		}
	}
	//enable menu selection using arrow keys
	else if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S)))
	{
		if (m_mouseOverIndex < m_menuItems.size() - 1)
			m_mouseOverIndex++;
	}
	else if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W)))
	{
		if (m_mouseOverIndex > 0)
			m_mouseOverIndex--;
	}
}
