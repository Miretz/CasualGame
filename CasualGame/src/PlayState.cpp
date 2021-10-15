#include "PlayState.h"

#include "Game.h"
#include "Sprite.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "GLRenderer.h"
#include "GLRaycaster.h"
#include "Clickable.h"
#include "PlayerInputManager.h"
#include "Utils.h"
#include "Config.h"

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
	m_fpsDisplay.setFont(g_fontLoader->getFont());
	m_fpsDisplay.setString("fps");
	m_fpsDisplay.setCharacterSize(32);
	m_fpsDisplay.setPosition(float(w) - 10.0f, 0.0f);
	m_fpsDisplay.setFillColor(sf::Color::Yellow);

	//Health display
	m_playerHealthDisplay.setFont(g_fontLoader->getFont());
	m_playerHealthDisplay.setString("health");
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
}

void PlayState::update(const float ft)
{
	double fts = static_cast<double>(ft / 1000.0f);

	//TODO: this is just a test
	//move aimed at sprite towards the player
	//moveAimedAtSprite(fts);

	//set indestructible until render calculation
	for (auto& outline : m_glRaycaster->getClickables())
	{
		outline.setDestructible(false);
	}

	//update health each frame
	m_playerHealthDisplay.setString("+ " + std::to_string(m_player->m_health));

	//update player movement
	m_inputManager->updatePlayerMovement(fts, m_player, m_levelReader->getLevel());

	//update player position on minimap
	float angle = std::atan2f(float(m_player->m_dirX), float(m_player->m_dirY));
	m_minimapPlayer.setPosition(float(m_player->m_posY) * g_playMinimapScale, float(m_player->m_posX) * g_playMinimapScale);
	m_minimapPlayer.setRotation((angle * 57.2957795f) + 90);

	//wobble gun
	if (m_inputManager->isMoving())
	{
		auto wobbleSpeed = fts * 10.0f;
		auto newGunPos = m_gunDisplay.getPosition();
		float DeltaHeight = static_cast<float>(sin(m_runningTime + wobbleSpeed) - sin(m_runningTime));
		newGunPos.y += DeltaHeight * 15.0f;
		m_runningTime += wobbleSpeed;
		m_gunDisplay.setPosition(newGunPos);
	}

	//reset gun texture
	if (!m_inputManager->isShooting())
	{
		m_gunDisplay.setTexture(&m_textureGun);
	}

}

void PlayState::draw(sf::RenderWindow& window)
{
	m_glRaycaster->draw(*m_player, *m_levelReader);

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
	m_minimapBackground.setSize(sf::Vector2f(levelSize * g_playMinimapScale, levelSize * g_playMinimapScale));
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
				wall.setPosition(x * g_playMinimapScale, y * g_playMinimapScale);
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
	auto spriteSize = m_levelReader->getSprites().size();
	for (size_t i = 0; i < spriteSize; i++)
	{
		auto sprite = m_levelReader->getSprites()[i];
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
	for (auto wall : m_minimapWallBuffer)
	{
		window.draw(wall);
	}

	//draw entities
	for (auto entity : m_minimapEntityBuffer)
	{
		window.draw(entity);
	}

	// Render Player on minimap
	window.draw(m_minimapPlayer);
}

void PlayState::drawGui(sf::RenderWindow& window)
{

	//draw hud clickable items
	for (auto& outline : m_glRaycaster->getClickables())
	{
		if (outline.containsVector(m_crosshair.getPosition()))
		{
			outline.draw(window);
			break;
		}
	}
	for (auto& outline : m_glRaycaster->getClickables())
	{
		outline.setVisible(false);
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

void PlayState::handleInput(const sf::Event& event, const sf::Vector2f& mousePosition, Game& game)
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
	m_inputManager->handleInput(event, mousePosition, game);

	//im shooting
	if (m_inputManager->isShooting())
	{
		m_gunDisplay.setTexture(&m_textureGun_fire);

		destroyAimedAtSprite();
	}
}

void PlayState::destroyAimedAtSprite()
{
	auto& clickables = m_glRaycaster->getClickables();

	for (size_t i = 0; i < clickables.size(); i++)
	{
		if (clickables[i].getDestructible() && clickables[i].containsVector(m_crosshair.getPosition()))
		{
			clickables[i].setDestructible(false);
			clickables[i].setVisible(false);
			if (clickables[i].getSpriteIndex() != -1)
			{
				m_levelReader->deleteSprite(clickables[i].getSpriteIndex());
				clickables[i].setSpriteIndex(-1);
			}
			return;
		}
	}
}

void PlayState::moveAimedAtSprite(const double fts)
{
	auto& clickables = m_glRaycaster->getClickables();
	auto& sprites = m_levelReader->getSprites();

	for (size_t i = 0; i < clickables.size(); i++)
	{
		if (clickables[i].getDestructible() && clickables[i].containsVector(m_crosshair.getPosition()))
		{
			if (clickables[i].getSpriteIndex() != -1)
			{
				auto x = sprites[clickables[i].getSpriteIndex()].x;
				auto y = sprites[clickables[i].getSpriteIndex()].y;

				x -= fts * 2.0 * m_player->m_dirX;
				y -= fts * 2.0 * m_player->m_dirY;

				m_levelReader->moveSprite(clickables[i].getSpriteIndex(), x, y);
			}
			return;
		}
	}
}
