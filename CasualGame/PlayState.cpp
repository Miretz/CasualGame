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
	m_glRaycaster->initialize(w, h, m_player, m_levelReader);

	//TODO create separate font loader
	m_font.loadFromFile(g_fontPath);

	//Fps display
	m_fpsDisplay.setFont(m_font);
	m_fpsDisplay.setString("fps");
	m_fpsDisplay.setCharacterSize(32);
	m_fpsDisplay.setPosition(float(w) - 10.0f, 0.0f);
	m_fpsDisplay.setColor(sf::Color::Yellow);

	//Health display
	m_playerHealthDisplay.setFont(m_font);
	m_playerHealthDisplay.setString("health");
	m_playerHealthDisplay.setCharacterSize(40);
	m_playerHealthDisplay.setPosition(10.0f, float(h) - m_playerHealthDisplay.getGlobalBounds().height * 3);
	m_playerHealthDisplay.setColor(sf::Color::White);

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

}

void PlayState::update(const float ft)
{
	double fts = static_cast<double>(ft / 1000.0f);
	
	//TODO: this is just a test
	//move aimed at sprite towards the player
	moveAimedAtSprite(fts);

	//set indestructible until render calculation
	for (auto& outline : m_glRaycaster->getClickables())
	{
		outline.setDestructible(false);
	}
	
	//update health each frame
	m_playerHealthDisplay.setString("+ " + std::to_string(m_player->m_health));

	//update player movement
	m_inputManager->updatePlayerMovement(fts, m_player, m_levelReader->getLevel());
	
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
	m_glRaycaster->draw();

	window.pushGLStates();

	//draw minimap
	drawMinimap(&window);

	//draw Gui elements
	drawGui(&window);

	window.popGLStates();

	m_glRaycaster->bindGlBuffers();

	window.display();
}



//Render minimap
void PlayState::drawMinimap(sf::RenderWindow* window) const
{

	//minimap background
	sf::RectangleShape minimapBg(sf::Vector2f(m_levelSize * g_playMinimapScale, m_levelSize * g_playMinimapScale));
	minimapBg.setPosition(0, 0);
	minimapBg.setFillColor(sf::Color(150, 150, 150, g_playMinimapTransparency));
	window->draw(minimapBg);

	//draw walls
	for (size_t y = 0; y < m_levelSize; y++)
	{
		for (size_t x = 0; x < m_levelSize; x++)
		{
			if (m_levelReader->getLevel()[y][x] > 0 && m_levelReader->getLevel()[y][x] < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(g_playMinimapScale, g_playMinimapScale));
				wall.setPosition(x * g_playMinimapScale, y * g_playMinimapScale);
				wall.setFillColor(sf::Color(0, 0, 0, g_playMinimapTransparency));
				window->draw(wall);
			}
		}
	}

	// Render entities on minimap
	for (size_t i = 0; i < m_spriteSize; i++)
	{
		sf::CircleShape object(g_playMinimapScale / 4.0f);
		object.setPosition(
			float(m_levelReader->getSprites()[i].y) * g_playMinimapScale,
			float(m_levelReader->getSprites()[i].x) * g_playMinimapScale);
		object.setOrigin(g_playMinimapScale / 2.0f, g_playMinimapScale / 2.0f);
		object.setFillColor(sf::Color(0, 0, 255, g_playMinimapTransparency));
		window->draw(object);
	}

	// Render Player on minimap
	{
		sf::CircleShape player(float(g_playMinimapScale), 3);
		player.setPosition(float(m_player->m_posY) * g_playMinimapScale, float(m_player->m_posX) * g_playMinimapScale);
		player.setFillColor(sf::Color(255, 255, 255, g_playMinimapTransparency));
		player.setOrigin(float(g_playMinimapScale), float(g_playMinimapScale));

		sf::RectangleShape player2(sf::Vector2f(g_playMinimapScale / 2.0f, g_playMinimapScale / 2.0f));
		player2.setPosition(float(m_player->m_posY) * g_playMinimapScale, float(m_player->m_posX) * g_playMinimapScale);
		player2.setFillColor(sf::Color(255, 255, 255, g_playMinimapTransparency));
		player2.setOrigin(g_playMinimapScale / 4.0f, -g_playMinimapScale / 2.0f);

		float angle = std::atan2f(float(m_player->m_dirX), float(m_player->m_dirY));
		player.setRotation((angle * 57.2957795f) + 90);
		player2.setRotation((angle * 57.2957795f) + 90);

		window->draw(player);
		window->draw(player2);
	}
}

void PlayState::drawGui(sf::RenderWindow* window)
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
	window->draw(m_gunDisplay);

	//draw fps display
	window->draw(m_fpsDisplay);

	//draw player health
	window->draw(m_playerHealthDisplay);

	//draw crosshair
	window->draw(m_crosshair);
}

void PlayState::handleInput(const sf::Event & event, const sf::Vector2f mousePosition, Game & game)
{
	//update fps from game
	m_fpsDisplay.setString(std::to_string(game.getFps()));
	m_fpsDisplay.setOrigin(m_fpsDisplay.getGlobalBounds().width, 0.0f);

	//escape to quit to main menu
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
	{
		m_glRaycaster->cleanup();
		game.changeState(GameStateName::MAINMENU);
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
