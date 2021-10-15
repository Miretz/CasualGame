#include "LevelReaderWriter.h"

#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>

#include "Config.h"
#include "Sprite.h"

namespace fs = std::filesystem;

LevelReaderWriter::LevelReaderWriter()
{

	loadLevel(g_defaultLevelFile, m_level, m_sprites);

	//load textures
	m_texture.resize(g_textureCount);
	m_sfmlTextures.resize(g_textureCount);

	// load all textures
	for (auto i = 0; i < g_textureCount; i++)
	{
		loadTexture(i, g_textureFiles[i]);

		sf::Image image;
		image.loadFromFile(g_textureFiles[i]);
		image.createMaskFromColor(sf::Color::Black);

		m_sfmlTextures[i].loadFromImage(image);
	}

	//swap texture X/Y
	//only works with square textures
	for (size_t i = 0; i < g_textureCount; i++)
		for (size_t x = 0; x < g_textureWidth; x++)
			for (size_t y = 0; y < x; y++)
				std::swap(m_texture[i][g_textureWidth * y + x], m_texture[i][g_textureWidth * x + y]);
}


void LevelReaderWriter::changeLevelTile(const int x, const int y, const int value)
{
	m_level[x][y] = value;
}

void LevelReaderWriter::moveSprite(const int index, const double x, const double y)
{
	m_sprites[index].x = x;
	m_sprites[index].y = y;
}

void LevelReaderWriter::createSprite(double x, double y, int texture)
{
	Sprite spr;
	spr.x = x;
	spr.y = y;
	spr.texture = texture;
	m_sprites.push_back(spr);
}

void LevelReaderWriter::deleteSprite(const int index)
{
	m_sprites.erase(m_sprites.begin() + index);
}

void LevelReaderWriter::loadDefaultLevel()
{
	m_level.clear();
	m_sprites.clear();

	std::vector<std::vector<int> >().swap(m_level);
	std::vector<Sprite>().swap(m_sprites);

	loadLevel(g_defaultLevelFile, m_level, m_sprites);
}

void LevelReaderWriter::loadCustomLevel(const std::string& levelName)
{
	m_level.clear();
	m_sprites.clear();

	std::vector<std::vector<int> >().swap(m_level);
	std::vector<Sprite>().swap(m_sprites);

	loadLevel(g_customLevelDirectory + levelName, m_level, m_sprites);
}

void LevelReaderWriter::saveCustomLevel(const std::string & levelName)
{

	//warning! overwrites the file if exists
	std::fstream file(g_customLevelDirectory + levelName, std::ios::out);

	//write walls
	for (size_t i = 0; i < m_level.size(); ++i)
	{
		for (size_t j = 0; j < m_level[i].size(); ++j)
		{
			file << m_level[i][j] << ",";
		}
		file << "\n";
	}
	file << "\n";

	//write sprites
	for (auto& spr : m_sprites)
	{
		file << spr.x << "," << spr.y << "," << spr.texture << "," << "\n";
	}
	file << "\n";
	file.close();
}

std::vector<std::string> LevelReaderWriter::getCustomLevels() const
{

	auto dpath = fs::path(g_customLevelDirectory);
	std::vector<std::string> entries;

	if (!fs::is_directory(dpath)) return entries;

	for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it)
	{
		if (!fs::is_directory(it->path()))
		{
			entries.emplace_back();
			std::string path = it->path().string();
			path.erase(0, std::string(g_customLevelDirectory).size());
			entries.back() = path;
		}
	}

	return entries;
}

void LevelReaderWriter::loadLevel(const std::string& path, std::vector<std::vector<int> >& level, std::vector<Sprite>& sprites) const
{
	std::ifstream file(path);

	//load walls
	std::string line;
	while (std::getline(file, line))
	{
		if (!file.good() || line.size() == 0)
		{
			break;
		}

		std::stringstream iss(line);
		std::string val;

		std::vector<int> rowVec;

		while (std::getline(iss, val, ','))
		{
			if (!iss.good())
				break;

			auto converted = 0;
			std::stringstream convertor(val);
			convertor >> converted;
			rowVec.push_back(converted);
		}

		level.push_back(rowVec);
	}

	//load sprites
	while (std::getline(file, line))
	{
		if (!file.good())
			break;

		std::stringstream iss(line);

		Sprite spr;
		for (auto col = 0; col < 3; col++)
		{
			std::string val;
			std::getline(iss, val, ',');
			if (!iss.good())
				break;

			std::stringstream convertor(val);

			switch (col)
			{
			case 0: convertor >> spr.x; break;
			case 1: convertor >> spr.y; break;
			case 2: convertor >> spr.texture; break;
			default: break;
			}
		}
		sprites.push_back(spr);
	}
}

// loads texture data from a file
void LevelReaderWriter::loadTexture(int index, const std::string& fileName)
{
	sf::Image image;
	image.loadFromFile(fileName);
	const sf::Uint8* imagePtr = image.getPixelsPtr();

	std::vector<sf::Uint32> texData;
	for (int i = 0; i < (g_textureHeight * g_textureWidth * 4); i += 4)
	{
		texData.push_back(
			imagePtr[i + 3] << 24 | imagePtr[i + 2] << 16 | imagePtr[i + 1] << 8 | imagePtr[i]);
	}

	m_texture[index] = texData;
}
