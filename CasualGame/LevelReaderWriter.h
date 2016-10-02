#pragma once

#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include "Sprite.h"

class LevelReaderWriter
{
public:
	LevelReaderWriter();
	virtual ~LevelReaderWriter() = default;

	const std::vector<std::vector<int> >& getLevel() const { return m_level; }
	const std::vector<Sprite>& getSprites() const { return m_sprites; };

	const std::vector<std::vector<sf::Uint32> >& getTextures() const { return m_texture; };
	const std::vector<sf::Uint32>& getTexture(const int index) const { return m_texture[index]; };

	void changeLevelTile(const int x, const int y, const int value);

	const sf::Texture* getTextureSfml(const int i) const { return &m_sfmlTextures[i]; };

	void moveSprite(const int index, const double x, const double y);
	void createSprite(double x, double y, int texture);
	void deleteSprite(const int index);

	void loadDefaultLevel();
	void loadCustomLevel(const std::string& levelName);
	void saveCustomLevel(const std::string& levelName);
	std::vector<std::string> getCustomLevels() const;

private:

	std::vector<std::vector<int> >m_level;
	std::vector<Sprite> m_sprites;
	std::vector<std::vector<sf::Uint32> > m_texture;
	std::vector<sf::Texture> m_sfmlTextures;

	void loadLevel(const std::string& path, std::vector<std::vector<int> >& level, std::vector<Sprite>& sprites) const;
	void generateTextures();
	void loadTexture(const int index, const std::string& fileName);
};

