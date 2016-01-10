#include "LevelReaderWriter.h"

LevelReaderWriter::LevelReaderWriter()
{
	loadLevel(levelFile, m_level);
	loadLevelSprites(levelSpriteFile, m_sprites);

	//texture generator 
	//generateTextures();

	//load textures
	auto numTextures = 13;
	m_texture.resize(numTextures);
	m_sfmlTextures.resize(numTextures);

	// load all textures
	for (int i = 0; i < numTextures; i++)
	{
		loadTexture(i, texPaths[i]);

		sf::Image image;
		image.loadFromFile(texPaths[i]);
		image.createMaskFromColor(sf::Color::Black);

		m_sfmlTextures[i].loadFromImage(image);
	}

	//swap texture X/Y
	//only works with square textures
	for (size_t i = 0; i < numTextures; i++)
		for (size_t x = 0; x < texWidth; x++)
			for (size_t y = 0; y < x; y++)
				std::swap(m_texture[i][texWidth * y + x], m_texture[i][texWidth * x + y]);
}


LevelReaderWriter::~LevelReaderWriter()
{
}

void LevelReaderWriter::changeLevelTile(const int x, const int y, const int value)
{
	m_level[x][y] = value;
}

void LevelReaderWriter::loadLevel(const std::string& path, std::vector<std::vector<int> >& level)
{
	std::ifstream file(path);

	std::string line;
	while (std::getline(file, line))
	{
		if (!file.good())
			break;

		std::stringstream iss(line);
		std::string val;

		std::vector<int> rowVec;

		while (std::getline(iss, val, ','))
		{
			if (!iss.good())
				break;

			int converted = 0;
			std::stringstream convertor(val);
			convertor >> converted;
			rowVec.push_back(converted);
		}

		level.push_back(rowVec);
	}
}

void LevelReaderWriter::loadLevelSprites(const std::string& path, std::vector<Sprite>& sprites)
{
	int row = 0;
	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
	{
		if (!file.good())
			break;

		std::stringstream iss(line);

		Sprite spr;
		for (int col = 0; col < 3; col++)
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


// generates some textures for testing
void LevelReaderWriter::generateTextures()
{
	for (int i = 0; i < 8; i++) m_texture[i].resize(texWidth * texHeight);
	for (int x = 0; x < texWidth; x++)
	{
		for (int y = 0; y < texHeight; y++)
		{
			int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
			//int xcolor = x * 256 / texWidth;
			int ycolor = y * 256 / texHeight;
			int xycolor = y * 128 / texHeight + x * 128 / texWidth;
			m_texture[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
			m_texture[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
			m_texture[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
			m_texture[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
			m_texture[4][texWidth * y + x] = 256 * xorcolor; //xor green
			m_texture[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
			m_texture[6][texWidth * y + x] = 65536 * ycolor; //red gradient
			m_texture[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
		}
	}
}

// loads texture data from a file
void LevelReaderWriter::loadTexture(int index, const std::string& fileName)
{
	sf::Image image;
	image.loadFromFile(fileName);
	const sf::Uint8* imagePtr = image.getPixelsPtr();

	std::vector<sf::Uint32> texData;
	for (int i = 0; i < (texHeight * texWidth * 4); i += 4)
	{
		texData.push_back(
			imagePtr[i + 3] << 24 | imagePtr[i + 2] << 16 | imagePtr[i + 1] << 8 | imagePtr[i]);
	}

	m_texture[index] = texData;
}
