#pragma once

#include <SFML/Graphics.hpp>

class GuiButton {
public:
	sf::Text text;
	sf::RectangleShape background;
	bool pressed;

	GuiButton(sf::Text Text, sf::RectangleShape Background) : text(Text), background(Background), pressed(false) {}
};

class LevelEditorGui
{
public:
	LevelEditorGui(const int x, const int y, const int width);
	~LevelEditorGui();

	void addButton(const std::string& text);
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition);
	void draw(sf::RenderWindow& window);

	const bool getPressed(const std::string& text);

	const int getWidth() const { return m_width; };
	const int getNumButtons() const { return m_buttons.size(); };
		
private:
	
	int m_padding = 10;

	int m_width;
	int m_xPos;
	int m_yPos;
	
	sf::Color m_idleColor = sf::Color::White;
	sf::Color m_hoverColor = sf::Color::Green;

	sf::Font m_font;
	std::vector<GuiButton> m_buttons;

};

