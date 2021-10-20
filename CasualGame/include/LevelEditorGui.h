#pragma once

#include <SFML/Graphics.hpp>

struct GuiButton
{
	sf::Text text;
	sf::RectangleShape background;
	bool pressed = false;

	GuiButton(const sf::Text& mText, const sf::RectangleShape& mBack) : text(mText), background(mBack) {}
};

class LevelEditorGui
{
public:
	LevelEditorGui(const int x, const int y, const int width);

	int addButton(const std::string& text);
	void setTexturedButton(const int index, const sf::Texture* texture);

	void addSpace();
	void handleInput(const sf::Event& event, const sf::Vector2f& mousepPosition);
	void draw(sf::RenderWindow& window) const;

	bool getPressed(const std::string& text);

	GuiButton& get(const int index) { return m_buttons[index]; };

	constexpr int getWidth() const { return m_width; };
	int getNumButtons() const { return (int)m_buttons.size(); };

private:

	int m_xPos;
	int m_yPos;
	int m_width;

	sf::Color m_idleColor = sf::Color::White;
	sf::Color m_hoverColor = sf::Color::Green;

	std::vector<GuiButton> m_buttons;

};

