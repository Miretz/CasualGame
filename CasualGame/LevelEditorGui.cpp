#include "LevelEditorGui.h"

#include "Config.h"

LevelEditorGui::LevelEditorGui(const int x, const int y, const int width) :
	m_xPos(x),
	m_yPos(y),
	m_width(width)
{
	m_font.loadFromFile(g_fontPath);
}

int LevelEditorGui::addButton(const std::string & text)
{
	sf::Text btnText;
	btnText.setFont(m_font);
	btnText.setString(text);
	btnText.setCharacterSize(22);
	btnText.setOrigin(0.0f, btnText.getGlobalBounds().height / 2.0f);
	btnText.setPosition(m_xPos + m_padding + 4.0f, float(m_yPos));
	btnText.setColor(m_idleColor);

	sf::RectangleShape rectShape(sf::Vector2f(float(m_width - 2 * m_padding), btnText.getGlobalBounds().height * 2.0f));
	rectShape.setPosition(float(m_xPos + m_padding), float(m_yPos));
	rectShape.setOutlineThickness(1);
	rectShape.setOutlineColor(m_idleColor);
	rectShape.setFillColor(sf::Color(0, 0, 0, 0));

	m_buttons.emplace_back(btnText, rectShape);

	m_yPos = m_yPos + m_padding + int(rectShape.getGlobalBounds().height);

	return m_buttons.size() - 1;
}

void LevelEditorGui::setTexturedButton(const int index, const sf::Texture* texture)
{
	GuiButton& btn = get(index);
	btn.background.setFillColor({ 255, 255, 255, 255 });
	btn.background.setTexture(texture);
}

void LevelEditorGui::addSpace()
{
	m_yPos += (2 * m_padding);
}

void LevelEditorGui::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition)
{
	auto mousePressed = event.type == sf::Event::MouseButtonPressed;

	for (auto& button : m_buttons)
	{
		if (button.background.getGlobalBounds().contains(mousepPosition))
		{
			button.background.setOutlineColor(m_hoverColor);
			button.text.setColor(m_hoverColor);
			button.pressed = mousePressed;
		}
		else
		{
			button.background.setOutlineColor(m_idleColor);
			button.text.setColor(m_idleColor);
		}
	}
}

void LevelEditorGui::draw(sf::RenderWindow & window)
{
	for (auto& button : m_buttons)
	{
		window.draw(button.background);
		window.draw(button.text);
	}
}

bool LevelEditorGui::getPressed(const std::string & text)
{
	for (auto& button : m_buttons)
	{
		if (button.text.getString() == text)
		{
			if (button.pressed)
			{
				button.pressed = false;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
