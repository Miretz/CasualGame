#include "GLRaycaster.h"

#include "GLRenderer.h"
#include "LevelReaderWriter.h"
#include "Player.h"
#include "Sprite.h"
#include "Clickable.h"
#include "Utils.h"
#include "Config.h"

#include <SFML/Graphics.hpp>

#include <functional>
#include <iterator>
#include <numeric>

GLRaycaster::GLRaycaster()
{
	m_glRenderer = std::make_unique<GLRenderer>();
}
GLRaycaster::~GLRaycaster()
{
	delete[] m_buffer;
	m_buffer = nullptr;

	delete[] m_ZBuffer;
	m_ZBuffer = nullptr;
}

void GLRaycaster::initialize(const int windowWidth, const int windowHeight, const int spriteSize)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_clickables.resize(spriteSize);

	m_ZBuffer = new double[windowWidth];
	m_buffer = new unsigned char[windowHeight * windowWidth * 4];

	m_glRenderer->init(m_buffer, windowWidth, windowHeight);

}

void GLRaycaster::draw(const Player& player, const LevelReaderWriter& levelReader)
{
	//calculate a new buffer
	calculateWalls(player, levelReader);
	calculateSprites(player, levelReader);

	m_glRenderer->draw(m_buffer, m_windowWidth, m_windowHeight);
	m_glRenderer->unbindBuffers();
}

void GLRaycaster::bindGlBuffers() const
{
	m_glRenderer->bindBuffers();
}

void GLRaycaster::cleanup() const
{
	m_glRenderer->cleanup();
}

void GLRaycaster::calculateWalls(const Player& player, const LevelReaderWriter& levelReader)
{

	const double rayPosX = player.m_posX;
	const double rayPosY = player.m_posY;

	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;

	//length of ray from current position to next x or y-side
	double sideDistX;
	double sideDistY;

	double perpWallDist;
	double wallX; //where exactly the wall was hit

	int side = 0; //was a NS or a EW wall hit?

	const auto& tex8 = levelReader.getTexture(8);//floor
	const auto& tex9 = levelReader.getTexture(9);//ceiling
	const auto& level = levelReader.getLevel();

	for (int x = 0; x < m_windowWidth; x++)
	{

		//calculate ray position and direction
		const double cameraX = 2.0 * x / m_windowWidth - 1.0; //x-coordinate in camera space

		const double rayDirX = player.m_dirX + player.m_planeX * cameraX;
		const double rayDirY = player.m_dirY + player.m_planeY * cameraX;

		//length of ray from one x or y-side to next x or y-side
		const double deltaDistX = std::sqrt(1.0 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		const double deltaDistY = std::sqrt(1.0 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

		//which box of the map we're in
		auto mapX = static_cast<int>(rayPosX);
		auto mapY = static_cast<int>(rayPosY);

		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (rayPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
		}

		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (rayPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
		}

		//perform ray walking
		while (level[mapX][mapY] == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
		}

		//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
		if (side == 0)
		{
			perpWallDist = std::abs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
			wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
		}
		else
		{
			perpWallDist = std::abs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);
			wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
		}
		wallX -= std::floor(wallX);

		//SET THE ZBUFFER FOR THE SPRITE CASTING
		m_ZBuffer[x] = perpWallDist; //perpendicular distance is used

		//Calculate height of line to draw on screen
		const auto lineHeight = static_cast<int>(std::abs(m_windowHeight / perpWallDist));

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + m_windowHeight / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + m_windowHeight / 2;
		if (drawEnd >= m_windowHeight)drawEnd = m_windowHeight - 1;

		//x coordinate on the texture
		auto texX = static_cast<int>(wallX * g_textureWidth);
		if (side == 0 && rayDirX > 0) texX = g_textureWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = g_textureWidth - texX - 1;

		const int texNum = level[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!
		const auto& texture = levelReader.getTexture(texNum);
		const auto texSize = static_cast<int>(texture.size());

		// draw the vertical stripe
		for (int y = drawStart; y < drawEnd + 1; y++)
		{
			int d = y * 256 - m_windowHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
			int texY = ((d * g_textureHeight) / lineHeight) / 256;
			int texNumY = g_textureHeight * texX + texY;

			if (texNumY < texSize && texNumY >= 0)
			{
				setPixel(x, y, texture[texNumY], side);
			}
		}

		//FLOOR CASTING
		double floorXWall = 0.0;
		double floorYWall = 0.0;

		if (side == 0 && rayDirX > 0)
		{
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		}
		else if (side == 0 && rayDirX <= 0)
		{
			floorXWall = mapX + 1.0;
			floorYWall = mapY + wallX;
		}
		else if (side == 1 && rayDirY > 0)
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY;
		}
		else
		{
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0;
		}

		if (drawEnd < 0) drawEnd = m_windowHeight; //becomes < 0 when the integer overflows

		//draw the floor from drawEnd to the bottom of the screen
		for (int y = drawEnd + 1; y < m_windowHeight; y++)
		{

			const double currentDist = m_windowHeight / (2.0 * y - m_windowHeight); //you could make a small lookup table for this instead
			const double weight = currentDist / perpWallDist;

			const double currentFloorX = weight * floorXWall + (1.0 - weight) * rayPosX;
			const double currentFloorY = weight * floorYWall + (1.0 - weight) * rayPosY;

			const int floorTexX = static_cast<int>(currentFloorX * g_textureWidth) % g_textureWidth;
			const int floorTexY = static_cast<int>(currentFloorY * g_textureHeight) % g_textureHeight;

			setPixel(x, y, tex8[g_textureWidth * floorTexY + floorTexX], 0);
			setPixel(x, m_windowHeight - y, tex9[g_textureWidth * floorTexY + floorTexX], 0);
			setPixel(x, m_windowHeight - y - 1, tex9[g_textureWidth * floorTexY + floorTexX], 0);

		}
	}
}


void GLRaycaster::calculateSprites(const Player& player, const LevelReaderWriter& levelReader)
{

	//SPRITE CASTING
	const auto& sprites = levelReader.getSprites();

	std::vector<int> spriteOrder(sprites.size());
	std::iota(spriteOrder.begin(), spriteOrder.end(), 0);

	std::vector<double> spriteDistance;
	std::transform(
		sprites.begin(),
		sprites.end(),
		std::back_inserter(spriteDistance),
		[&player](const auto& sprite) -> double
		{
			return ((player.m_posX - sprite.x) * (player.m_posX - sprite.x) +
				(player.m_posY - sprite.y) * (player.m_posY - sprite.y));
		});

	Utils::combSort(spriteOrder, spriteDistance, static_cast<int>(sprites.size()));

	//after sorting the sprites, do the projection and draw them
	for (size_t i = 0; i < sprites.size(); i++)
	{

		//translate sprite position to relative to camera
		const double spriteX = sprites[spriteOrder[i]].x - player.m_posX;
		const double spriteY = sprites[spriteOrder[i]].y - player.m_posY;

		const double invDet = 1.0 / (player.m_planeX * player.m_dirY - player.m_dirX * player.m_planeY); //required for correct matrix multiplication
		const double transformX = invDet * (player.m_dirY * spriteX - player.m_dirX * spriteY);
		const double transformY = invDet * (-player.m_planeY * spriteX + player.m_planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D       
		const auto spriteScreenX = static_cast<int>((m_windowWidth / 2) * (1 + transformX / transformY));

		//calculate height of the sprite on screen
		const int spriteHeight = abs(int(m_windowHeight / transformY)); //using "transformY" instead of the real distance prevents fisheye

		//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + m_windowHeight / 2;
		int drawEndY = spriteHeight / 2 + m_windowHeight / 2;

		//calculate width of the sprite
		int spriteWidth = spriteHeight;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		int drawEndX = spriteWidth / 2 + spriteScreenX;

		const int texNr = sprites[spriteOrder[i]].texture;
		const auto& textureData = levelReader.getTexture(texNr);
		const auto texSize = static_cast<int>(textureData.size());

		//setup clickables
		m_clickables[i].update(
			sf::Vector2f(static_cast<float>(spriteWidth) / 2.0f, static_cast<float>(spriteHeight)),
			sf::Vector2f(static_cast<float>(drawStartX) + static_cast<float>(spriteWidth) / 4.0f, static_cast<float>(drawStartY)));
		m_clickables[i].setSpriteIndex(spriteOrder[i]);

		//limit drawstart and drawend
		if (drawStartY < 0) drawStartY = 0;
		if (drawEndY >= m_windowHeight) drawEndY = m_windowHeight - 1;
		if (drawStartX < 0) drawStartX = 0;
		if (drawEndX >= m_windowWidth) drawEndX = m_windowWidth - 1;

		//loop through every vertical stripe of the sprite on screen
		for (int stripe = drawStartX; stripe < drawEndX; stripe++)
		{

			const int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * g_textureWidth / spriteWidth) / 256;

			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if (transformY > 0 && stripe > 0 && stripe < m_windowWidth && transformY < m_ZBuffer[stripe])
			{

				m_clickables[i].setVisible(texNr != 12);
				m_clickables[i].setDestructible(texNr != 12);
				const int factor = m_windowHeight * 128 + spriteHeight * 128;

				//for every pixel of the current stripe
				for (int y = drawStartY; y < drawEndY; y++)
				{
					const int d = y * 256 - factor; //256 and 128 factors to avoid floats
					const int texY = ((d * g_textureHeight) / spriteHeight) / 256;
					const int texPix = g_textureWidth * texX + texY;

					// prevent exception when accessing tex pixel out of range
					if (texPix < texSize && texPix > 0)
					{
						sf::Uint32 color = textureData[texPix]; //get current color from the texture
						// black color is transparent
						if ((color & 0x00FFFFFF) != 0)
						{
							setPixel(stripe, y, color, 0);
						}
					}
				}

			}
		}

	}
}


void GLRaycaster::setPixel(int x, int y, const sf::Uint32 colorRgba, unsigned int style)
{
	auto colors = (sf::Uint8*)&colorRgba;
	const int index = (y * m_windowWidth + x) * 3;
	m_buffer[index] = colors[0] >> style;
	m_buffer[index + 1] = colors[1] >> style;
	m_buffer[index + 2] = colors[2] >> style;
}

