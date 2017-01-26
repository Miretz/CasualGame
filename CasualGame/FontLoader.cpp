#include "FontLoader.h"

FontLoader::FontLoader(const char* path)
{
	m_font.loadFromFile(path);
}
