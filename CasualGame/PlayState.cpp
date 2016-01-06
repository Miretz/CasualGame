#include "PlayState.h"

#define texWidth 512
#define texHeight 512

const int PlayState::m_level[24][24] =
{
	{ 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7 },
	{ 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7 },
	{ 4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7 },
	{ 4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7 },
	{ 4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7 },
	{ 4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7 },
	{ 4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1 },
	{ 4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8 },
	{ 4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1 },
	{ 4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8 },
	{ 4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1 },
	{ 4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1 },
	{ 6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6 },
	{ 8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4 },
	{ 6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6 },
	{ 4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3 },
	{ 4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2 },
	{ 4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2 },
	{ 4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2 },
	{ 4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2 },
	{ 4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2 },
	{ 4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2 },
	{ 4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2 },
	{ 4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3 }
};

PlayState::PlayState() : m_posX(22.0), m_posY(11.5), m_dirX(-1.0), m_dirY(0.0), m_planeX(0.0), m_planeY(0.66)
{

	//texture generator 
	/*
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
	*/

	loadTexture(0, "resources/textures/stonebricks.png");
	loadTexture(1, "resources/textures/concbase.png");
	loadTexture(2, "resources/textures/metalblocks.png");
	loadTexture(3, "resources/textures/concbricks.png");
	loadTexture(4, "resources/textures/concblocks.png");
	loadTexture(5, "resources/textures/redbricks.png");
	loadTexture(6, "resources/textures/metalwall.png");
	loadTexture(7, "resources/textures/redbricks2.png");
	loadTexture(8, "resources/textures/diagonal.png");
	loadTexture(9, "resources/textures/stoneblocks.png");

	//swap texture X/Y
	//only works with square textures
	for (size_t i = 0; i < 10; i++)
		for (size_t x = 0; x < texWidth; x++)
			for (size_t y = 0; y < x; y++)
				std::swap(m_texture[i][texWidth * y + x], m_texture[i][texWidth * x + y]);


	m_buffer = new sf::Vertex[windowHeight];
}


PlayState::~PlayState()
{
	//Empty
}

void PlayState::loadTexture(int index, const std::string& fileName)
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


void PlayState::update(float ft)
{
	// inputs
	double moveSpeed = static_cast<double>(ft/1000) * 5.0; //the constant value is in squares/second
	double rotSpeed = static_cast<double>(ft/1000) * 3.0; //the constant value is in radians/second
	
	// process movement
	if (m_movement[0])
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = m_dirX;
		m_dirX = m_dirX * cos(rotSpeed) - m_dirY * sin(rotSpeed);
		m_dirY = oldDirX * sin(rotSpeed) + m_dirY * cos(rotSpeed);
		double oldPlaneX = m_planeX;
		m_planeX = m_planeX * cos(rotSpeed) - m_planeY * sin(rotSpeed);
		m_planeY = oldPlaneX * sin(rotSpeed) + m_planeY * cos(rotSpeed);
	}
	if (m_movement[1]) 
	{
		//both camera direction and camera plane must be rotated
		double oldDirX = m_dirX;
		m_dirX = m_dirX * cos(-rotSpeed) - m_dirY * sin(-rotSpeed);
		m_dirY = oldDirX * sin(-rotSpeed) + m_dirY * cos(-rotSpeed);
		double oldPlaneX = m_planeX;
		m_planeX = m_planeX * cos(-rotSpeed) - m_planeY * sin(-rotSpeed);
		m_planeY = oldPlaneX * sin(-rotSpeed) + m_planeY * cos(-rotSpeed);
	}
	if (m_movement[2])
	{
		if (m_level[int(m_posX + m_dirX * moveSpeed)][int(m_posY)] == false) m_posX += m_dirX * moveSpeed;
		if (m_level[int(m_posX)][int(m_posY + m_dirY * moveSpeed)] == false) m_posY += m_dirY * moveSpeed;
	}
	if (m_movement[3])
	{
		if (m_level[int(m_posX - m_dirX * moveSpeed)][int(m_posY)] == false) m_posX -= m_dirX * moveSpeed;
		if (m_level[int(m_posX)][int(m_posY - m_dirY * moveSpeed)] == false) m_posY -= m_dirY * moveSpeed;
	}

}

void PlayState::draw(sf::RenderWindow& window)
{
	window.clear();

	//2d raycaster
	for (int x = 0; x < windowWidth; x++)
	{
		//calculate ray position and direction
		double cameraX = 2 * x / static_cast<double>(windowWidth) - 1; //x-coordinate in camera space
		double rayPosX = m_posX;
		double rayPosY = m_posY;
		double rayDirX = m_dirX + m_planeX * cameraX;
		double rayDirY = m_dirY + m_planeY * cameraX;

		//which box of the map we're in
		int mapX = static_cast<int>(rayPosX);
		int mapY = static_cast<int>(rayPosY);

		//length of ray from one x or y-side to next x or y-side
		double rayDirXsq = rayDirX * rayDirX;
		double rayDirYsq = rayDirY * rayDirY;
		double deltaDistX = sqrt(1 + rayDirYsq / rayDirXsq);
		double deltaDistY = sqrt(1 + rayDirXsq / rayDirYsq);
		
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

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

		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?

		//perform DDA
		while (hit == 0)
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
			//Check if ray has hit a wall
			if (m_level[mapX][mapY] > 0) hit = 1;
		}

		//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
		double perpWallDist;
		if (side == 0)
			perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
		else
			perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

		//Calculate height of line to draw on screen
		int lineHeight = static_cast<int>(std::abs(windowHeight / perpWallDist));

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + windowHeight / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + windowHeight / 2;
		if (drawEnd >= windowHeight)drawEnd = windowHeight - 1;

		//texturing calculations
		int texNum = m_level[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

		double wallX; //where exactly the wall was hit
		if (side == 1)
		{
			wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
		}
		else
		{
			wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
		}
		wallX -= floor((wallX));

		//x coordinate on the texture
		int texX = static_cast<int>(wallX * texWidth);
		if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

		//darker sides
		for (int y = drawStart; y < drawEnd; y++)
		{
			
			int d = y * 256 - windowHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
			int texY = ((d * texHeight) / lineHeight) / 256;

			int texNumY = texHeight * texX + texY;

			if (texNumY < m_texture[texNum].size())
			{
				sf::Uint32 color = m_texture[texNum][texNumY];

				//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
				if (side == 1) color = (color >> 1) & 8355711;

				m_buffer[y] = sf::Vertex{ sf::Vector2f(x, y),
							  sf::Color(color & 0x000000ff, (color & 0x0000ff00) >> 8, (color & 0x00ff0000) >> 16, 255) };

			}
		}

		//FLOOR CASTING
		double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

		//4 different wall directions possible
		if (side == 0 && rayDirX > 0)
		{
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		}
		else if (side == 0 && rayDirX < 0)
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

		double distWall, distPlayer, currentDist;

		distWall = perpWallDist;
		distPlayer = 0.0;

		if (drawEnd < 0) drawEnd = windowHeight; //becomes < 0 when the integer overflows

		//draw the floor from drawEnd to the bottom of the screen
		for (int y = drawEnd + 1; y < windowHeight; y++)
		{
			currentDist = windowHeight / (2.0 * y - windowHeight); //you could make a small lookup table for this instead

			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			double currentFloorX = weight * floorXWall + (1.0 - weight) * m_posX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * m_posY;

			int floorTexX, floorTexY;
			floorTexX = int(currentFloorX * texWidth) % texWidth;
			floorTexY = int(currentFloorY * texHeight) % texHeight;

			//floor
			sf::Uint32 color1 = (m_texture[8][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
			sf::Uint32 color2 = (m_texture[9][texWidth * floorTexY + floorTexX] >> 1) & 8355711;

			m_buffer[y] = sf::Vertex{ sf::Vector2f(x, y),
				sf::Color(color1 & 0x000000ff, (color1 & 0x0000ff00) >> 8, (color1 & 0x00ff0000) >> 16, 255) };
			m_buffer[windowHeight - y] = sf::Vertex{ sf::Vector2f(x, windowHeight - y),
				sf::Color(color2 & 0x000000ff, (color2 & 0x0000ff00) >> 8, (color2 & 0x00ff0000) >> 16, 255) };
		}

		window.draw(m_buffer, windowHeight, sf::Points);

		//clear
		for (int i = 0; i < windowHeight; i++)
		{
			m_buffer[i].color = sf::Color::Black;
		}

	}
	window.display();
}

void PlayState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game)
{

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed) 
	{
		// handle controls
		if (event.key.code == sf::Keyboard::Left)
		{
			m_movement[0] = true;
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_movement[1] = true;
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			m_movement[2] = true;
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			m_movement[3] = true;
		}
			
	}

	if (event.type == sf::Event::KeyReleased)
	{
		// handle controls
		if (event.key.code == sf::Keyboard::Left)
		{
			m_movement[0] = false;
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			m_movement[1] = false;
		}
		if (event.key.code == sf::Keyboard::Up)
		{
			m_movement[2] = false;
		}
		if (event.key.code == sf::Keyboard::Down)
		{
			m_movement[3] = false;
		}

		if (event.key.code == sf::Keyboard::Escape)
		{
			game.changeState(Game::GameStateName::MAINMENU);
		}

	}
	
}
