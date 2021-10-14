#include "LevelEditorState.h"

#include "Sprite.h"
#include "Game.h"
#include "Player.h"
#include "LevelReaderWriter.h"
#include "LevelEditorGui.h"
#include "Config.h"

LevelEditorState::LevelEditorState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader) :
	m_windowWidth(w),
	m_windowHeight(h),
	m_player(move(player)),
	m_levelReader(move(levelReader))
{
	m_scale = float(h - 30) / m_levelReader->getLevel().size();

	m_statusBar.setFont(g_fontLoader->getFont());
	m_statusBar.setString(g_editorTxtModeWall);
	m_statusBar.setCharacterSize(32);

	m_statusBar.setPosition(10.f, float(h));
	m_statusBar.setOrigin(0.0f, m_statusBar.getGlobalBounds().height * 2.0f);

	m_statusBar.setFillColor(sf::Color::Black);

	m_customLevels = m_levelReader->getCustomLevels();

	//Gui
	m_gui = std::make_unique<LevelEditorGui>(w - g_editorMenuWidth + 1, 10, g_editorMenuWidth);
	m_gui->addButton(g_editorTxtSwitchMode);
	m_gui->addButton(g_editorTxtLoadDefault);

	m_gui->addSpace();
	for (auto& cl : m_customLevels)
	{
		m_gui->addButton(cl);
	}
	m_gui->addSpace();
	m_filenameGuiIndex = m_gui->addButton(m_customLevelName);
	m_gui->addButton(g_editorTxtSave);
	m_gui->addSpace();
	m_gui->addButton(g_editorTxtQuit);
	m_gui->addSpace();
	m_gui->addSpace();

	m_textureButtonId = m_gui->addButton(g_editorTxtTexture);
	m_gui->setTexturedButton(m_textureButtonId, m_levelReader->getTextureSfml(m_selectedTexture - 1));

	m_spriteButtonId = m_gui->addButton(g_editorTxtSprite);
	m_gui->setTexturedButton(m_spriteButtonId, m_levelReader->getTextureSfml(m_selectedSprite - 1));

	m_gui->get(m_spriteButtonId).background.setSize({ 100,100 });
}

void LevelEditorState::update(const float ft)
{
	//Empty
}

void LevelEditorState::draw(sf::RenderWindow & window)
{

	window.clear();

	//Render map background
	sf::RectangleShape minimapBg({ float(m_windowWidth), float(m_windowHeight) });
	minimapBg.setPosition(0, 0);
	minimapBg.setFillColor(sf::Color(150, 150, 150, 255));
	window.draw(minimapBg);

	// Render Player on map
	drawPlayer(window);

	//walls
	drawWalls(window);

	// Render entities on minimap
	drawSprites(window);

	// Render placement square under mouse
	if (m_editEntities && (m_entitySelected != -1))
	{
		sf::RectangleShape mouseRect(sf::Vector2f(m_scale - 1.0f, m_scale - 1.0f));
		mouseRect.setPosition(m_mousePos.x, m_mousePos.y);
		mouseRect.setOrigin(m_scale / 2.0f, m_scale / 2.0f);
		mouseRect.setOutlineThickness(1);
		mouseRect.setOutlineColor(sf::Color(255, 255, 255, 80));
		mouseRect.setFillColor(sf::Color(0, 0, 0, 0));
		window.draw(mouseRect);
	}

	//draw status bar
	window.draw(m_statusBar);

	//draw Gui Menu
	m_gui->draw(window);

	window.display();

}

void LevelEditorState::handleInput(const sf::Event & event, const sf::Vector2f& mousepPosition, Game & game)
{

	m_mousePos = mousepPosition;

	//filename editing mode
	if (m_filenameMode)
	{
		handleInputField(event);
		return; // ignore all other events
	}

	//gui events
	m_gui->handleInput(event, mousepPosition);
	handleMenuCallbacks(event, game);

	//is the mouse inside the editor area
	const auto levelSize = m_levelReader->getLevel().size();
	auto mouseInEditor = (mousepPosition.x < levelSize * m_scale);

	//process button press
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			if (m_entitySelected != -1)
			{
				m_entitySelected = -1;
			}
			else
			{
				game.changeState(GameStateName::MAINMENU);
			}
		}
		if (event.key.code == sf::Keyboard::Space)
		{
			toggleMode();
		}

		//entity move with arrow keys
		if (m_editEntities && mouseInEditor)
		{
			if (m_entitySelected != -1)
			{
				if (event.key.code == sf::Keyboard::Left)
				{
					Sprite spr = m_levelReader->getSprites()[m_entitySelected];
					m_levelReader->moveSprite(m_entitySelected, spr.x, spr.y -= 0.1);
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					Sprite spr = m_levelReader->getSprites()[m_entitySelected];
					m_levelReader->moveSprite(m_entitySelected, spr.x, spr.y += 0.1);
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					Sprite spr = m_levelReader->getSprites()[m_entitySelected];
					m_levelReader->moveSprite(m_entitySelected, spr.x -= 0.1, spr.y);
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					Sprite spr = m_levelReader->getSprites()[m_entitySelected];
					m_levelReader->moveSprite(m_entitySelected, spr.x += 0.1, spr.y);
				}
				if (event.key.code == sf::Keyboard::Delete)
				{
					m_levelReader->deleteSprite(m_entitySelected);
					m_entitySelected = -1;
				}
			}
		}
	}

	//process mouse click
	if (event.type == sf::Event::MouseButtonPressed && mouseInEditor)
	{

		const auto x = mousepPosition.x / m_scale;
		const auto y = mousepPosition.y / m_scale;

		//SPRITE EDITING
		if (m_editEntities)
		{

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (m_entitySelected != -1)
				{
					//move sprite data
					m_levelReader->moveSprite(m_entitySelected, y, x);
					m_entitySelected = -1;
				}
				else
				{
					const auto spritesSize = m_levelReader->getSprites().size();
					for (size_t i = 0; i < spritesSize; i++)
					{
						sf::RectangleShape object(sf::Vector2f(m_scale - 2.0f, m_scale - 2.0f));
						object.setPosition(float(m_levelReader->getSprites()[i].y) * m_scale, float(m_levelReader->getSprites()[i].x) * m_scale);
						object.setOrigin(m_scale / 2.0f, m_scale / 2.0f);
						if (object.getGlobalBounds().contains(mousepPosition))
						{
							m_entitySelected = i;
						}
					}
				}
			}
			else
			{
				m_levelReader->createSprite(y, x, m_selectedSprite - 1);
			}

		}
		// WALL EDITING
		else
		{
			auto xi = static_cast<int>(x);
			auto yi = static_cast<int>(y);

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//set wall texture with left click
				m_levelReader->changeLevelTile(yi, xi, m_selectedTexture);
			}
			else
			{
				//delete walls with right click			
				//Do not allow to delete the level outer walls, this breaks the raycaster
				if (xi == 0 || xi == levelSize - 1)
				{
					m_levelReader->changeLevelTile(yi, xi, m_selectedTexture);
				}
				else if (yi == 0 || yi == levelSize - 1)
				{
					m_levelReader->changeLevelTile(yi, xi, m_selectedTexture);
				}
				else
				{
					m_levelReader->changeLevelTile(yi, xi, 0);
				}
			}
		}
	}
}

void LevelEditorState::toggleMode()
{
	m_editEntities = !m_editEntities;

	if (!m_editEntities)
	{
		m_statusBar.setString(g_editorTxtModeWall);
	}
	else
	{
		m_statusBar.setString(g_editorTxtModeEntity);
	}
}

void LevelEditorState::resetPlayer() const
{
	m_player->m_posX = 22.0;
	m_player->m_posY = 11.5;
	m_player->m_dirX = -1.0;
	m_player->m_dirY = 0.0;
	m_player->m_planeX = 0.0;
	m_player->m_planeY = 0.66;
}

void LevelEditorState::drawPlayer(sf::RenderWindow & window) const
{

	const auto posX = float(m_player->m_posY) * m_scale;
	const auto posY = float(m_player->m_posX) * m_scale;

	sf::CircleShape player(g_editorPlayerArrowScale, 3);
	player.setPosition(posX, posY);
	player.setFillColor(sf::Color(255, 255, 255, 255));
	player.setOrigin(g_editorPlayerArrowScale, g_editorPlayerArrowScale);

	sf::RectangleShape player2(sf::Vector2f(g_editorPlayerArrowScale / 2.0f, g_editorPlayerArrowScale / 2.0f));
	player2.setPosition(posX, posY);
	player2.setFillColor(sf::Color(255, 255, 255, 255));
	player2.setOrigin(g_editorPlayerArrowScale / 4.0f, -g_editorPlayerArrowScale / 2.0f);

	auto angle = std::atan2f(float(m_player->m_dirX), float(m_player->m_dirY));
	player.setRotation((angle * 57.2957795f) + 90.0f);
	player2.setRotation((angle * 57.2957795f) + 90.0f);

	window.draw(player);
	window.draw(player2);
}

void LevelEditorState::drawWalls(sf::RenderWindow & window) const
{
	const auto levelSize = m_levelReader->getLevel().size();
	for (size_t y = 0; y < levelSize; y++)
	{
		for (size_t x = 0; x < levelSize; x++)
		{
			auto id = m_levelReader->getLevel()[y][x];
			if (id > 0 && id < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(m_scale - 2.0f, m_scale - 2.0f));
				wall.setPosition(x * m_scale, y * m_scale);
				wall.setTexture(m_levelReader->getTextureSfml(id - 1));
				wall.setOutlineThickness(2);
				if (!m_editEntities)
				{
					wall.setOutlineColor(sf::Color(0, 0, 0, 255));
				}
				else
				{
					wall.setFillColor(sf::Color(255, 255, 255, 100));
					wall.setOutlineColor(sf::Color(0, 0, 0, 100));
				}
				window.draw(wall);
			}
		}
	}
}

void LevelEditorState::drawSprites(sf::RenderWindow & window) const
{
	const auto spritesSize = m_levelReader->getSprites().size();
	for (size_t i = 0; i < spritesSize; i++)
	{
		sf::RectangleShape object(sf::Vector2f(m_scale - 2.0f, m_scale - 2.0f));
		object.setPosition(float(m_levelReader->getSprites()[i].y) * m_scale, float(m_levelReader->getSprites()[i].x) * m_scale);
		object.setOrigin(m_scale / 2.0f, m_scale / 2.0f);
		object.setOutlineThickness(2);

		if (m_editEntities)
		{
			if (i == m_entitySelected)
			{
				object.setOutlineColor(sf::Color(0, 255, 0, 255));
			}
			else
			{
				object.setOutlineColor(sf::Color(255, 255, 255, 128));
			}
		}
		else
		{
			object.setOutlineColor(sf::Color(255, 255, 255, 0));
		}

		object.setTexture(m_levelReader->getTextureSfml(m_levelReader->getSprites()[i].texture));
		window.draw(object);
	}
}

void LevelEditorState::handleInputField(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return)
		{
			m_filenameMode = !m_filenameMode;
		}
		if (event.key.code == sf::Keyboard::BackSpace)
		{
			if (m_customLevelName.size() > 0)
			{
				m_customLevelName.erase(m_customLevelName.size() - 1, m_customLevelName.size());
				m_gui->get(m_filenameGuiIndex).text.setString(m_customLevelName);
			}
		}
	}
	else if (event.type == sf::Event::TextEntered)
	{
		if ((event.text.unicode > 65 && event.text.unicode < 123) || (event.text.unicode > 48 && event.text.unicode < 58))
		{
			m_customLevelName += static_cast<char>(event.text.unicode);
			m_gui->get(m_filenameGuiIndex).text.setString(m_customLevelName);
		}
	}
}

void LevelEditorState::handleMenuCallbacks(const sf::Event & event, Game & game)
{
	//process gui events first
	if (m_gui->getPressed(g_editorTxtSwitchMode))
	{
		toggleMode();
	}
	if (m_gui->getPressed(g_editorTxtLoadDefault))
	{

		resetPlayer();

		m_levelReader->loadDefaultLevel();
	}
	for (auto& cl : m_customLevels)
	{
		if (m_gui->getPressed(cl))
		{

			resetPlayer();

			m_levelReader->loadCustomLevel(cl);
		}
	}
	if (m_gui->getPressed(m_customLevelName))
	{
		m_customLevelName = "";
		m_gui->get(m_filenameGuiIndex).text.setString(m_customLevelName);
		m_filenameMode = !m_filenameMode;
	}
	if (m_gui->getPressed(g_editorTxtSave) && m_customLevelName.size() > 0 && m_customLevelName != "<enter filename>")
	{
		m_levelReader->saveCustomLevel(m_customLevelName + ".txt");
		game.changeState(GameStateName::LEVEL_EDITOR);
	}
	if (m_gui->getPressed(g_editorTxtQuit))
	{
		game.changeState(GameStateName::MAINMENU);
	}
	if (m_gui->getPressed(g_editorTxtTexture))
	{
		// 1 ... 8 - walls
		// 9, 10 - floor, ceiling
		// 11,12,13 - barrel, pillar, light
		// In fact you have to substract 1 for the level 
		if (m_selectedTexture < 8)
		{
			m_selectedTexture += 1;
		}
		else
		{
			m_selectedTexture = 1;
		}

		m_gui->setTexturedButton(m_textureButtonId, m_levelReader->getTextureSfml(m_selectedTexture - 1));
	}
	if (m_gui->getPressed(g_editorTxtSprite))
	{
		// 11,12,13 - barrel, pillar, light
		if (m_selectedSprite < 13)
		{
			m_selectedSprite += 1;
		}
		else
		{
			m_selectedSprite = 11;
		}

		m_gui->setTexturedButton(m_spriteButtonId, m_levelReader->getTextureSfml(m_selectedSprite - 1));
	}
}


