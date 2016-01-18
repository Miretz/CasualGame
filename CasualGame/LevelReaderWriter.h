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

static const int texWidth = 128;
static const int texHeight = 128;

static const auto levelFile = "resources/levels/level1.txt";
static const auto levelSpriteFile = "resources/levels/level1_sprites.txt";
static const auto customLevelDir = "resources/levels/custom/";

static const int numTextures = 13;
static const std::string texPaths[13] = {
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

class LevelReaderWriter {
public:
	LevelReaderWriter();
	~LevelReaderWriter();
	
	const std::vector<std::vector<int> >& getLevel() const { return m_level;  }
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
	const std::vector<std::string> getCustomLevels();

private:
	
	std::vector<std::vector<int> >m_level;
	std::vector<Sprite> m_sprites;
	std::vector<std::vector<sf::Uint32> > m_texture;
	std::vector<sf::Texture> m_sfmlTextures;
	
	void loadLevel(const std::string& path, std::vector<std::vector<int> >& level, std::vector<Sprite>& sprites);
	void generateTextures();
	void loadTexture(const int index, const std::string& fileName);
};

