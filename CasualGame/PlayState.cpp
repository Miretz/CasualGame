#include "PlayState.h"

RandomGenerator PlayState::gen = RandomGenerator();

PlayState::PlayState()
{
	//colors for background
	for (int i = 0; i < 4; ++i)
	{
		m_bgColors.emplace_back(0, 0, 0, 150);
	}

	// followers vector
	for (int i = 0; i < 300; ++i)
	{
		sf::CircleShape circle(1.f);
		circle.setFillColor(sf::Color::White);
		circle.setPosition(gen.randomInt(0, GameConfig::windowWidth), gen.randomInt(0, GameConfig::windowHeight));
		m_followers.push_back(circle);
	}
}


PlayState::~PlayState()
{
}

void PlayState::update(float ft)
{
	int changePlace = gen.randomInt(0, m_followers.size());
	float dirX = gen.randomFloat(-5.0f, 5.0f);
	float dirY = gen.randomFloat(-5.0f, 5.0f);

	for (int i = 0; i < m_followers.size(); ++i)
	{
		if (i == changePlace)
		{
			m_followers[i].move(dirX, dirY);
		}
	}

	const int colorMax = 100;

	//slight bg color update
	for (int i = 0; i < m_bgColors.size(); ++i)
	{

		if (gen.randomInt(0, 100) < 40)
		{
			m_bgColors[i].r += gen.randomInt(-2, 2);
			m_bgColors[i].g += gen.randomInt(-2, 2);
			m_bgColors[i].b += gen.randomInt(-2, 2);
		}

		//limit color max
		if (m_bgColors[i].r > colorMax) m_bgColors[i].r = colorMax;
		if (m_bgColors[i].g > colorMax) m_bgColors[i].g = colorMax;
		if (m_bgColors[i].b > colorMax) m_bgColors[i].b = colorMax;
		if (m_bgColors[i].r < 0) m_bgColors[i].r = 0;
		if (m_bgColors[i].g < 0) m_bgColors[i].g = 0;
		if (m_bgColors[i].b < 0) m_bgColors[i].b = 0;
	}
}

void PlayState::draw(sf::RenderWindow & window)
{
	window.clear();

	//draw followers 
	for (const auto follower : m_followers)
	{
		window.draw(follower);
	}

	//bg rectangle
	sf::Vertex bgRect[] =
	{
		sf::Vertex(sf::Vector2f(0.f,0.f), m_bgColors[0]),
		sf::Vertex(sf::Vector2f(0.f,GameConfig::windowHeight), m_bgColors[1]),
		sf::Vertex(sf::Vector2f(GameConfig::windowWidth, GameConfig::windowHeight), m_bgColors[2]),
		sf::Vertex(sf::Vector2f(GameConfig::windowWidth, 0.0f), m_bgColors[3])
	};
	window.draw(bgRect, 4, sf::Quads);
	
	window.display();

}

void PlayState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game)
{

	m_mousePos = mousepPosition;

	//escape go to main menu
	if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
	{
		game.changeState(Game::GameStateName::MAINMENU);
	}

}
