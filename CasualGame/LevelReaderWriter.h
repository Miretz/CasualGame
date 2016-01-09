#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "Sprite.h"

static constexpr auto levelFile = "resources/levels/level1.txt";
static constexpr auto levelSpriteFile = "resources/levels/level1_sprites.txt";

class LevelReaderWriter
{
public:
	LevelReaderWriter();
	~LevelReaderWriter();
	
	const std::vector<std::vector<int> >& const getLevel() { return m_level;  }
	const std::vector<Sprite>& const getSprites() { return m_sprites; };

private:

	std::vector<std::vector<int> >m_level;
	std::vector<Sprite> m_sprites;

	void loadLevel(const std::string& path, std::vector<std::vector<int> >& level);
	void loadLevelSprites(const std::string& path, std::vector<Sprite>& sprites);
};

