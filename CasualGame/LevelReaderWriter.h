#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include "Sprite.h"

static constexpr auto texWidth = 128;
static constexpr auto texHeight = 128;

static constexpr auto levelFile = "resources/levels/level1.txt";
static constexpr auto levelSpriteFile = "resources/levels/level1_sprites.txt";

static constexpr auto customLevelDir = "resources/levels/custom/";

class LevelReaderWriter
{
public:
	LevelReaderWriter();
	~LevelReaderWriter();
	
	const std::vector<std::vector<int> >& const getLevel() { return m_level;  }
	const std::vector<Sprite>& const getSprites() { return m_sprites; };
	
	const std::vector<std::vector<sf::Uint32> >& const getTextures() { return m_texture; };
	const std::vector<sf::Uint32>& const getTexture(const int index) { return m_texture[index]; };
	
	void changeLevelTile(const int x, const int y, const int value);

	const sf::Texture* const getTextureSfml(const int i) { return &m_sfmlTextures[i]; };

	void moveSprite(const int index, const double x, const double y);
	void createSprite(double x, double y, int texture);
	void deleteSprite(const int index);

	void reloadLevel();
	void loadCustomLevel(const std::string& levelName);
	void saveCustomLevel(const std::string& levelName);
	const std::vector<std::string> getCustomLevels();

private:

	const std::string texPaths[13] = { 
		"resources/textures/stonebricks.png",//1
		"resources/textures/concbase.png",//2
		"resources/textures/metalblocks.png",//3
		"resources/textures/concbricks.png",//4
		"resources/textures/concblocks.png",//5
		"resources/textures/redbricks.png",//6
		"resources/textures/metalwall.png",//7
		"resources/textures/redbricks2.png",//8
		"resources/textures/diagonal.png", //floor 9
		"resources/textures/stoneblocks.png", //ceiling 10
		"resources/sprites/barrel.png",//11 
		"resources/sprites/pillar.png",//12
		"resources/sprites/greenlight.png"//13
	};

	std::vector<std::vector<int> >m_level;
	std::vector<Sprite> m_sprites;
	std::vector<std::vector<sf::Uint32> > m_texture;
	std::vector<sf::Texture> m_sfmlTextures;
	
	void loadLevel(const std::string& path, std::vector<std::vector<int> >& level);
	void loadLevelSprites(const std::string& path, std::vector<Sprite>& sprites);

	void generateTextures();
	void loadTexture(int index, const std::string& fileName);
};

