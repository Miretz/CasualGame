#include "LevelEditorGui.h"


LevelEditorGui::LevelEditorGui(const int x, const int y, const int width) :
m_xPos(x), m_yPos(y), m_width(width)
{
	//Empty
	m_font.loadFromFile("resources/font/OtherF.ttf");
}

LevelEditorGui::~LevelEditorGui()
{
}

void LevelEditorGui::addButton(const std::string & text)
{
	sf::Text btnText;
	btnText.setFont(m_font);
	btnText.setString(text);
	btnText.setCharacterSize(20);
	btnText.setOrigin(0.0f, btnText.getGlobalBounds().height / 2.0f);
	btnText.setPosition(m_xPos + m_padding + 4.0f, m_yPos);
	btnText.setColor(m_idleColor);

	sf::RectangleShape rectShape(sf::Vector2f((m_width - 2 * m_padding), btnText.getGlobalBounds().height * 2.0f));
	rectShape.setPosition(m_xPos + m_padding, m_yPos);
	rectShape.setOutlineThickness(1);
	rectShape.setOutlineColor(m_idleColor);
	rectShape.setFillColor(sf::Color(0,0,0,0));

	m_buttons.emplace_back(btnText, rectShape);

	m_yPos = m_yPos + m_padding + rectShape.getGlobalBounds().height;
}

void LevelEditorGui::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition)
{
	bool mousePressed = event.type == sf::Event::MouseButtonPressed;

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

const bool LevelEditorGui::getPressed(const std::string & text)
{
	for (auto& button : m_buttons)
	{
		if (button.text.getString() == text)
		{
			if (button.pressed) {
				button.pressed = false;
				return true;
			} 
			else {
				return false;
			}
		}
	}
	return false;
}
