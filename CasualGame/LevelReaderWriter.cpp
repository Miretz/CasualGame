#include "LevelReaderWriter.h"

LevelReaderWriter::LevelReaderWriter()
{
	loadLevel(levelFile, m_level);
	loadLevelSprites(levelSpriteFile, m_sprites);
}


LevelReaderWriter::~LevelReaderWriter()
{
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
