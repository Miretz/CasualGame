#include "MainMenuState.h"

#include "Game.h"
#include "RandomGenerator.h"
#include "Config.h"

RandomGenerator MainMenuState::gen = RandomGenerator();

MainMenuState::MainMenuState(const int w, const int h) :
	m_windowWidth(w),
	m_windowHeight(h)
{

	// Game Title
	sf::Color textColor = sf::Color::White;

	m_titleText.setFont(g_fontLoader.getFont());
	m_titleText.setString(g_mainTxtTitle);
	m_titleText.setCharacterSize(50);
	m_titleText.setPosition(static_cast<float>(m_windowWidth) / 2.0f, 200);
	m_titleText.setOrigin(m_titleText.getGlobalBounds().width / 2.0f, m_titleText.getGlobalBounds().height / 2.0f);
	m_titleText.setFillColor(textColor);

	// Menu Items
	// Start Game
	sf::Text startGame;
	startGame.setFont(g_fontLoader.getFont());
	startGame.setString(g_mainTxtStartGame);
	startGame.setCharacterSize(30);
	startGame.setPosition(static_cast<float>(m_windowWidth) / 2.0f, 300);
	startGame.setOrigin(startGame.getGlobalBounds().width / 2.0f, startGame.getGlobalBounds().height / 2.0f);
	startGame.setFillColor(textColor);

	m_menuItems.push_back(startGame);

	// Restart Game
	sf::Text restartGame;
	restartGame.setFont(g_fontLoader.getFont());
	restartGame.setString(g_mainTxtRestartGame);
	restartGame.setCharacterSize(30);
	restartGame.setPosition(static_cast<float>(m_windowWidth) / 2.0f, 350);
	restartGame.setOrigin(restartGame.getGlobalBounds().width / 2.0f, restartGame.getGlobalBounds().height / 2.0f);
	restartGame.setFillColor(textColor);

	m_menuItems.push_back(restartGame);

	// Level Editor 
	sf::Text levelEditor;
	levelEditor.setFont(g_fontLoader.getFont());
	levelEditor.setString(g_mainTxtLevelEditor);
	levelEditor.setCharacterSize(30);
	levelEditor.setPosition(static_cast<float>(m_windowWidth) / 2.0f, 400);
	levelEditor.setOrigin(levelEditor.getGlobalBounds().width / 2.0f, levelEditor.getGlobalBounds().height / 2.0f);
	levelEditor.setFillColor(textColor);

	m_menuItems.push_back(levelEditor);

	//Toggle Fullscreen
	sf::Text switchFullscreen;
	switchFullscreen.setFont(g_fontLoader.getFont());
	switchFullscreen.setString(g_mainTxtToggleFullscreen);
	switchFullscreen.setCharacterSize(30);
	switchFullscreen.setPosition(static_cast<float>(m_windowWidth) / 2.0f, 450);
	switchFullscreen.setOrigin(switchFullscreen.getGlobalBounds().width / 2.0f, switchFullscreen.getGlobalBounds().height / 2.0f);
	switchFullscreen.setFillColor(textColor);

	m_menuItems.push_back(switchFullscreen);

	//Quit Game
	sf::Text quitGame;
	quitGame.setFont(g_fontLoader.getFont());
	quitGame.setString(g_mainTxtQuit);
	quitGame.setCharacterSize(30);
	quitGame.setPosition(static_cast<float>(m_windowWidth) / 2.0f, 500);
	quitGame.setOrigin(quitGame.getGlobalBounds().width / 2.0f, quitGame.getGlobalBounds().height / 2.0f);
	quitGame.setFillColor(textColor);

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

void MainMenuState::update([[maybe_unused]] const float ft)
{

	sf::Transform rotation;
	rotation.rotate(0.01f, static_cast<float>(m_windowWidth) / 2.0f, static_cast<float>(m_windowHeight) / 2.0f);
	for (auto& follower : m_followers)
	{
		follower.setPosition(rotation.transformPoint(follower.getPosition()));
	}

	const int colorMax = 100;

	//slight bg color update
	for (auto& bgColor : m_bgColors)
	{

		if (gen.randomInt(0, 100) < 10)
		{
			bgColor.r += gen.randomInt(-2, 2);
			bgColor.g += gen.randomInt(-2, 2);
			bgColor.b += gen.randomInt(-2, 2);
		}

		//limit color max
		if (bgColor.r > colorMax) bgColor.r = colorMax;
		if (bgColor.g > colorMax) bgColor.g = colorMax;
		if (bgColor.b > colorMax) bgColor.b = colorMax;
		if (bgColor.r <= 0) bgColor.r = 0;
		if (bgColor.g <= 0) bgColor.g = 0;
		if (bgColor.b <= 0) bgColor.b = 0;
	}

}

void MainMenuState::draw(sf::RenderWindow& window)
{
	window.clear();

	//draw followers 
	for (const auto& follower : m_followers)
	{
		window.draw(follower);
	}

	//bg rectangle
	std::vector<sf::Vertex> bgRect{
		sf::Vertex(sf::Vector2f(0.f,0.f), m_bgColors[0]),
		sf::Vertex(sf::Vector2f(0.f,static_cast<float>(m_windowHeight)), m_bgColors[1]),
		sf::Vertex(sf::Vector2f(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)), m_bgColors[2]),
		sf::Vertex(sf::Vector2f(static_cast<float>(m_windowWidth), 0.0f), m_bgColors[3])
	};
	window.draw(&bgRect[0], 4, sf::Quads);

	window.draw(m_titleText);

	for (size_t i = 0; i < m_menuItems.size(); ++i)
	{
		sf::Text item = m_menuItems[i];

		sf::FloatRect boundingBox = item.getGlobalBounds();

		// draw selection box
		if (sf::FloatRect boundsWithPadding{
			boundingBox.left - m_padding / 2.0f,
			boundingBox.top - m_padding / 2.0f,
			boundingBox.width + m_padding,
			boundingBox.height + m_padding };
			boundsWithPadding.contains(m_mousePos) || m_mouseOverIndex == i)
		{
			m_mouseOverIndex = static_cast<int>(i);

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

void MainMenuState::handleInput(const sf::Event& event, const sf::Vector2f& mousePosition, Game& game)
{

	m_mousePos = mousePosition;

	//exit
	if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
	{
		game.changeState(GameStateName::QUIT);
	}
	//left mouse or enter button
	else if (((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return)) ||
		((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)))
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
	else if ((event.type == sf::Event::KeyPressed) && ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W)) && (m_mouseOverIndex > 0))
	{
		m_mouseOverIndex--;
	}
}
