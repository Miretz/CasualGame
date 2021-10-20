#include "PlayState.h"

#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "GLRaycaster.h"
#include "Clickable.h"
#include "PlayerInputManager.h"
#include "Utils.h"
#include "Config.h"

#include <algorithm>

PlayState::PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader) :
	m_player(move(player)),
	m_levelReader(move(levelReader))
{

	m_levelSize = m_levelReader->getLevel().size();
	m_spriteSize = m_levelReader->getSprites().size();

	m_inputManager = std::make_unique<PlayerInputManager>();

	m_glRaycaster = std::make_unique<GLRaycaster>();
	m_glRaycaster->initialize(w, h, static_cast<int>(m_spriteSize));

	//Fps display
	m_fpsDisplay.setFont(g_fontLoader.getFont());
	m_fpsDisplay.setString("fps");
	m_fpsDisplay.setCharacterSize(32);
	m_fpsDisplay.setPosition(float(w) - 10.0f, 0.0f);
	m_fpsDisplay.setFillColor(sf::Color::Yellow);

	//Health display
	m_playerHealthDisplay.setFont(g_fontLoader.getFont());
	m_playerHealthDisplay.setString("+ " + std::to_string(m_player->m_health));
	m_playerHealthDisplay.setCharacterSize(40);
	m_playerHealthDisplay.setPosition(10.0f, float(h) - m_playerHealthDisplay.getGlobalBounds().height * 3);
	m_playerHealthDisplay.setFillColor(sf::Color::White);

	//Gun display
	//idle texture
	sf::Image gunImg;
	gunImg.loadFromFile(g_gunSprite);
	gunImg.createMaskFromColor(sf::Color::Black);
	m_textureGun.loadFromImage(gunImg);

	//fire texture
	sf::Image gunImgFire;
	gunImgFire.loadFromFile(g_gunSprite_fire);
	gunImgFire.createMaskFromColor(sf::Color::Black);
	m_textureGun_fire.loadFromImage(gunImgFire);

	//set gun size, position and texture
	m_gunDisplay.setSize({ float(g_textureWidth * 2), float(g_textureHeight * 2) });
	m_gunDisplay.setPosition({ float(w / 2 - g_textureWidth), float(h - g_textureHeight * 2 + 30) });
	m_gunDisplay.setTexture(&m_textureGun);

	//crosshair
	m_crosshair.setRadius(2.0f);
	m_crosshair.setPosition({ float(w / 2) - 1.0f, float(h / 2) - 1.0f });
	m_crosshair.setFillColor(sf::Color::White);

	generateMinimap();

	m_glRaycaster->update(*m_player, *m_levelReader);
}

void PlayState::update(const float ft)
{
	const auto fts = static_cast<double>(ft / 1000.0f);

	//update player movement
	m_inputManager->updatePlayerMovement(fts, m_player, m_levelReader->getLevel());

	//wobble gun
	if (m_inputManager->isMoving())
	{
		//update player position on minimap
		const auto angle = std::atan2(m_player->m_dirX, m_player->m_dirY);
		m_minimapPlayer.setPosition(static_cast<float>(m_player->m_posY) * g_playMinimapScale, static_cast<float>(m_player->m_posX) * g_playMinimapScale);
		m_minimapPlayer.setRotation(static_cast<float>(angle * 57.2957795) + 90.f);

		const auto wobbleSpeed = fts * 10.0;
		const auto deltaHeight = static_cast<float>(std::sin(m_runningTime + wobbleSpeed) - std::sin(m_runningTime));
		m_runningTime += wobbleSpeed;
		m_gunDisplay.setPosition(m_gunDisplay.getPosition() + sf::Vector2f{ 0.f, deltaHeight * 15.0f });
	}

	//reset gun texture
	if (!m_inputManager->isShooting())
	{
		m_gunDisplay.setTexture(&m_textureGun);
	}
}

void PlayState::draw(sf::RenderWindow& window)
{
	if (m_inputManager->isMoving() || m_inputManager->isShooting())
	{
		m_glRaycaster->update(*m_player, *m_levelReader);
	}

	m_glRaycaster->draw();

	window.pushGLStates();

	//draw minimap
	drawMinimap(window);

	//draw Gui elements
	drawGui(window);

	window.popGLStates();

	m_glRaycaster->bindGlBuffers();

	window.display();
}

void PlayState::generateMinimap()
{
	// Minimap player arrow
	// resize it to 5 points
	m_minimapPlayer.setPointCount(7);

	// define the points
	m_minimapPlayer.setPoint(0, sf::Vector2f(0, 0));
	m_minimapPlayer.setPoint(1, sf::Vector2f(10, 10));
	m_minimapPlayer.setPoint(2, sf::Vector2f(5, 10));
	m_minimapPlayer.setPoint(3, sf::Vector2f(5, 15));
	m_minimapPlayer.setPoint(4, sf::Vector2f(-5, 15));
	m_minimapPlayer.setPoint(5, sf::Vector2f(-5, 10));
	m_minimapPlayer.setPoint(6, sf::Vector2f(-10, 10));

	m_minimapPlayer.setFillColor(sf::Color(255, 255, 255, g_playMinimapTransparency));

	auto levelSize = m_levelReader->getLevel().size();

	//Minimap background
	m_minimapBackground.setSize(sf::Vector2f(static_cast<float>(levelSize) * g_playMinimapScale, static_cast<float>(levelSize) * g_playMinimapScale));
	m_minimapBackground.setPosition(0, 0);
	m_minimapBackground.setFillColor(sf::Color(150, 150, 150, g_playMinimapTransparency));

	// Minimap walls
	for (size_t y = 0; y < levelSize; y++)
	{
		for (size_t x = 0; x < levelSize; x++)
		{
			if (m_levelReader->getLevel()[y][x] > 0 && m_levelReader->getLevel()[y][x] < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(g_playMinimapScale, g_playMinimapScale));
				wall.setPosition(static_cast<float>(x) * g_playMinimapScale, static_cast<float>(y) * g_playMinimapScale);
				wall.setFillColor(sf::Color(0, 0, 0, g_playMinimapTransparency));
				m_minimapWallBuffer.push_back(wall);
			}
		}
	}

	updateMinimapEntities();
}

void PlayState::updateMinimapEntities()
{
	// Clear
	std::vector<sf::CircleShape>().swap(m_minimapEntityBuffer);

	// Entities on minimap
	for (size_t i = 0; i < m_levelReader->getSprites().size(); i++)
	{
		const auto& sprite = m_levelReader->getSprites()[i];

		sf::CircleShape object(g_playMinimapScale / 4.0f);
		object.setPosition(
			float(sprite.y) * g_playMinimapScale,
			float(sprite.x) * g_playMinimapScale);
		object.setOrigin(g_playMinimapScale / 2.0f, g_playMinimapScale / 2.0f);
		object.setFillColor(sf::Color(0, 0, 255, g_playMinimapTransparency));
		m_minimapEntityBuffer.push_back(object);
	}
}

//Render minimap
void PlayState::drawMinimap(sf::RenderWindow& window) const
{

	//minimap background
	window.draw(m_minimapBackground);

	//draw walls
	for (const auto& wall : m_minimapWallBuffer)
	{
		window.draw(wall);
	}

	//draw entities
	for (const auto& entity : m_minimapEntityBuffer)
	{
		window.draw(entity);
	}

	// Render Player on minimap
	window.draw(m_minimapPlayer);
}

void PlayState::drawGui(sf::RenderWindow& window) const
{

	//draw hud clickable items
	for (auto& outline : m_glRaycaster->getClickables())
	{
		if (outline.containsVector(m_crosshair.getPosition()))
		{
			outline.draw(window);
		}
	}

	//draw gun
	window.draw(m_gunDisplay);

	//draw fps display
	window.draw(m_fpsDisplay);

	//draw player health
	window.draw(m_playerHealthDisplay);

	//draw crosshair
	window.draw(m_crosshair);
}

void PlayState::handleInput(const sf::Event& event, [[maybe_unused]] const sf::Vector2f& mousePosition, Game& game)
{
	//update fps from game
	m_fpsDisplay.setString(std::to_string(game.getFps()));
	m_fpsDisplay.setOrigin(m_fpsDisplay.getGlobalBounds().width, 0.0f);

	//escape to quit to main menu
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
	{
		m_glRaycaster->cleanup();
		game.changeState(GameStateName::MAINMENU);
		return;
	}

	//send events to player controller
	m_inputManager->handleInput(event);

	//im shooting
	if (m_inputManager->isShooting())
	{
		m_gunDisplay.setTexture(&m_textureGun_fire);

		destroyAimedAtSprite();
	}
}

void PlayState::destroyAimedAtSprite() const
{
	for (auto i = m_glRaycaster->getClickables().size(); i-- > 0;)
	{
		auto& clickable = m_glRaycaster->getClickables()[i];
		if (clickable.getDestructible() && clickable.containsVector(m_crosshair.getPosition()))
		{
			clickable.setDestructible(false);
			clickable.setVisible(false);
			if (clickable.getSpriteIndex() != -1)
			{
				m_levelReader->deleteSprite(clickable.getSpriteIndex());
				clickable.setSpriteIndex(-1);
			}
			return;
		}
	}
}
