#pragma once

#include <SFML/Graphics.hpp>

class FontLoader
{
public:
	explicit FontLoader(const char* path);
	virtual ~FontLoader() = default;

	const sf::Font& getFont() const { return m_font; }

private:
	sf::Font m_font;
};

