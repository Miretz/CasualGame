#include "PlayState.h"

PlayState::PlayState(const int w, const int h, std::shared_ptr<LevelReaderWriter> levelReader) : m_posX(22.0), m_posY(11.5),
m_dirX(-1.0), m_dirY(0.0),
m_planeX(0.0), m_planeY(0.66),
m_windowWidth(w), m_windowHeight(h),
m_levelReader(move(levelReader)),
m_levelRef(m_levelReader->getLevel()),
m_spriteRef(m_levelReader->getSprites()),
m_levelSize(m_levelReader->getLevel().size()),
m_spriteSize(m_levelReader->getSprites().size())
{
	m_buffer.resize(h * w);
	m_ZBuffer.resize(w);

	m_spriteOrder.resize(m_levelReader->getSprites().size());
	m_spriteDistance.resize(m_levelReader->getSprites().size());
}

PlayState::~PlayState()
{
	//Empty
}

void PlayState::update(const float ft)
{
	if (m_forward || m_backward || m_left || m_right)
	{
		// convert ms to seconds
		double fts = static_cast<double>(ft / 1000);
		double moveSpeed = fts * 5.0; //the constant value is in squares/second
		double rotSpeed = fts * 3.0; //the constant value is in radians/second
									 // process movement
		if (m_left)
		{
			//both camera direction and camera plane must be rotated
			double oldDirX = m_dirX;
			m_dirX = m_dirX * cos(rotSpeed) - m_dirY * sin(rotSpeed);
			m_dirY = oldDirX * sin(rotSpeed) + m_dirY * cos(rotSpeed);
			double oldPlaneX = m_planeX;
			m_planeX = m_planeX * cos(rotSpeed) - m_planeY * sin(rotSpeed);
			m_planeY = oldPlaneX * sin(rotSpeed) + m_planeY * cos(rotSpeed);
		}
		if (m_right)
		{
			//both camera direction and camera plane must be rotated
			double oldDirX = m_dirX;
			m_dirX = m_dirX * cos(-rotSpeed) - m_dirY * sin(-rotSpeed);
			m_dirY = oldDirX * sin(-rotSpeed) + m_dirY * cos(-rotSpeed);
			double oldPlaneX = m_planeX;
			m_planeX = m_planeX * cos(-rotSpeed) - m_planeY * sin(-rotSpeed);
			m_planeY = oldPlaneX * sin(-rotSpeed) + m_planeY * cos(-rotSpeed);
		}
		if (m_forward)
		{
			if (m_levelRef[int(m_posX + m_dirX * moveSpeed)][int(m_posY)] == false) m_posX += m_dirX * moveSpeed;
			if (m_levelRef[int(m_posX)][int(m_posY + m_dirY * moveSpeed)] == false) m_posY += m_dirY * moveSpeed;
		}
		if (m_backward)
		{
			if (m_levelRef[int(m_posX - m_dirX * moveSpeed)][int(m_posY)] == false) m_posX -= m_dirX * moveSpeed;
			if (m_levelRef[int(m_posX)][int(m_posY - m_dirY * moveSpeed)] == false) m_posY -= m_dirY * moveSpeed;
		}
	}
}

void PlayState::draw(sf::RenderWindow& window)
{
	window.clear();

	//calculate walls, floors and ceilings
	const int wallIndex = calculateWalls();
	window.draw(&m_buffer[0], wallIndex, sf::Points);
	
	//calculate sprites
	const int sprIndex = calculateSprites();
	window.draw(&m_buffer[0], sprIndex, sf::Points);

	//draw minimap
	drawMinimap(&window);

	window.display();
	
}

const int PlayState::calculateWalls()
{
	int pixIndex = 0;

	//2d raycaster
	for (int x = 0; x < m_windowWidth; x++)
	{
		//calculate ray position and direction
		double cameraX = 2 * x / double(m_windowWidth) - 1; //x-coordinate in camera space
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
			if (m_levelRef[mapX][mapY] > 0) hit = 1;
		}

		//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
		double perpWallDist;
		if (side == 0)
			perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
		else
			perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

		//Calculate height of line to draw on screen
		int lineHeight = static_cast<int>(std::abs(m_windowHeight / perpWallDist));

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + m_windowHeight / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + m_windowHeight / 2;
		if (drawEnd >= m_windowHeight)drawEnd = m_windowHeight - 1;

		//texturing calculations
		int texNum = m_levelRef[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

		double wallX; //where exactly the wall was hit
		if (side == 1)
		{
			wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
		}
		else
		{
			wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
		}
		wallX -= floor(wallX);

		//x coordinate on the texture
		int texX = static_cast<int>(wallX * texWidth);
		if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

		//darker sides
		for (int y = drawStart; y < drawEnd; y++)
		{

			int d = y * 256 - m_windowHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
			int texY = ((d * texHeight) / lineHeight) / 256;

			int texNumY = texHeight * texX + texY;

			if (texNumY < m_levelReader->getTexture(texNum).size())
			{
				sf::Uint32 color = m_levelReader->getTexture(texNum)[texNumY];

				//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
				if (side == 1) color = (color >> 1) & 8355711;

				m_buffer[pixIndex].position = sf::Vector2f((float)x, (float)y);
				m_buffer[pixIndex++].color = toColor(color);
			}
		}

		//SET THE ZBUFFER FOR THE SPRITE CASTING
		m_ZBuffer[x] = perpWallDist; //perpendicular distance is used

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

		double distWall = perpWallDist;
		double distPlayer = 0.0;

		if (drawEnd < 0) drawEnd = m_windowHeight; //becomes < 0 when the integer overflows

												   //draw the floor from drawEnd to the bottom of the screen
		for (int y = drawEnd + 1; y < m_windowHeight; y++)
		{
			double currentDist = m_windowHeight / (2.0 * y - m_windowHeight); //you could make a small lookup table for this instead

			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			double currentFloorX = weight * floorXWall + (1.0 - weight) * m_posX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * m_posY;

			int floorTexX, floorTexY;
			floorTexX = int(currentFloorX * texWidth) % texWidth;
			floorTexY = int(currentFloorY * texHeight) % texHeight;

			//floor textures
			sf::Uint32 color1 = m_levelReader->getTexture(8)[texWidth * floorTexY + floorTexX];
			sf::Uint32 color2 = m_levelReader->getTexture(9)[texWidth * floorTexY + floorTexX];

			m_buffer[pixIndex].position = sf::Vector2f((float)x, (float)y);
			m_buffer[pixIndex++].color = toColor(color1);
			m_buffer[pixIndex].position = sf::Vector2f((float)x, float(m_windowHeight - y));
			m_buffer[pixIndex++].color = toColor(color2);

		}
	}

	return pixIndex;
}

const int PlayState::calculateSprites()
{
	
	int pixIndex = 0;
	
	//SPRITE CASTING
	//sort sprites from far to close
	for (size_t i = 0; i < m_spriteSize; i++)
	{
		m_spriteOrder[i] = i;
		m_spriteDistance[i] = ((m_posX - m_spriteRef[i].x) * (m_posX - m_spriteRef[i].x) + (m_posY - m_spriteRef[i].y) * (m_posY - m_spriteRef[i].y)); //sqrt not taken, unneeded
	}
	combSort(m_spriteOrder, m_spriteDistance, m_spriteRef.size());

	//after sorting the sprites, do the projection and draw them
	for (size_t i = 0; i < m_spriteRef.size(); i++)
	{
		//translate sprite position to relative to camera
		double spriteX = m_spriteRef[m_spriteOrder[i]].x - m_posX;
		double spriteY = m_spriteRef[m_spriteOrder[i]].y - m_posY;

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		double invDet = 1.0 / (m_planeX * m_dirY - m_dirX * m_planeY); //required for correct matrix multiplication

		double transformX = invDet * (m_dirY * spriteX - m_dirX * spriteY);
		double transformY = invDet * (-m_planeY * spriteX + m_planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D       

		int spriteScreenX = int((m_windowWidth / 2) * (1 + transformX / transformY));

		//calculate height of the sprite on screen
		int spriteHeight = abs(int(m_windowHeight / (transformY))); //using "transformY" instead of the real distance prevents fisheye

																	//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + m_windowHeight / 2;
		if (drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + m_windowHeight / 2;
		if (drawEndY >= m_windowHeight) drawEndY = m_windowHeight - 1;

		//calculate width of the sprite
		int spriteWidth = abs(int(m_windowHeight / (transformY)));
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= m_windowWidth) drawEndX = m_windowWidth - 1;

		//loop through every vertical stripe of the sprite on screen
		for (int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if (transformY > 0 && stripe > 0 && stripe < m_windowWidth && transformY < m_ZBuffer[stripe])
			{
				for (int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
				{
					int d = (y)* 256 - m_windowHeight * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
					int texY = ((d * texHeight) / spriteHeight) / 256;

					int texPix = texWidth * texX + texY;
					int texNr = m_spriteRef[m_spriteOrder[i]].texture;

					if (texPix < m_levelReader->getTexture(texNr).size()) // prevent exception when accessing tex pixel out of range
					{
						sf::Uint32 color = m_levelReader->getTexture(texNr)[texPix]; //get current color from the texture
						if ((color & 0x00FFFFFF) != 0) // black is invisible!!!
						{
							m_buffer[pixIndex].position = sf::Vector2f((float)stripe, (float)y);
							m_buffer[pixIndex++].color = toColor(color);
						}
					}
				}

			}
		}
	}

	return pixIndex;
}

//Render minimap
void PlayState::drawMinimap(sf::RenderWindow* window)
{
	
	//minimap background
	sf::RectangleShape minimapBg(sf::Vector2f(m_levelSize * minimapScale, m_levelSize * minimapScale));
	minimapBg.setPosition(0, 0);
	minimapBg.setFillColor(sf::Color(150, 150, 150, minimapTransparency));
	window->draw(minimapBg);

	for (int y = 0; y < m_levelSize; y++)
	{
		for (int x = 0; x < m_levelSize; x++)
		{
			//draw walls
			if (m_levelRef[y][x] > 0 && m_levelRef[y][x] < 9)
			{
				sf::RectangleShape wall(sf::Vector2f(minimapScale, minimapScale));
				wall.setPosition(x * minimapScale, y * minimapScale);
				wall.setFillColor(sf::Color(0, 0, 0, minimapTransparency));
				window->draw(wall);
			}
		}
	}
	// Render entities on minimap
	for (size_t i = 0; i < m_spriteSize; i++)
	{
		sf::CircleShape object(minimapScale / 4.0f);
		object.setPosition(m_spriteRef[i].y * minimapScale, m_spriteRef[i].x * minimapScale);
		object.setOrigin(minimapScale / 2.0f, minimapScale / 2.0f);
		object.setFillColor(sf::Color(0, 0, 255, minimapTransparency));
		window->draw(object);
	}
	// Render Player on minimap
	{
		sf::CircleShape player(minimapScale, 3);
		player.setPosition(m_posY * minimapScale, m_posX * minimapScale);
		player.setFillColor(sf::Color(255, 255, 255, minimapTransparency));
		player.setOrigin(minimapScale, minimapScale);

		sf::RectangleShape player2(sf::Vector2f(minimapScale / 2.0f, minimapScale / 2.0f));
		player2.setPosition(m_posY * minimapScale, m_posX * minimapScale);
		player2.setFillColor(sf::Color(255, 255, 255, minimapTransparency));
		player2.setOrigin(minimapScale / 4.0f, -minimapScale / 2.0f);

		float angle = std::atan2f(m_dirX, m_dirY);
		player.setRotation((angle * 57.2957795f) + 90);
		player2.setRotation((angle * 57.2957795f) + 90);

		window->draw(player);
		window->draw(player2);
	}
}

const sf::Color PlayState::toColor(sf::Uint32 colorRgba)
{
	return sf::Color(colorRgba & 0x000000ff, (colorRgba & 0x0000ff00) >> 8, (colorRgba & 0x00ff0000) >> 16, 255);
}


void PlayState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game)
{

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed)
	{
		// handle controls
		if (event.key.code == sf::Keyboard::Left)
		{
			m_left = true;
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			m_right = true;
		}
		else if (event.key.code == sf::Keyboard::Up)
		{
			m_forward = true;
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			m_backward = true;
		}

	}

	if (event.type == sf::Event::KeyReleased)
	{
		// handle controls
		if (event.key.code == sf::Keyboard::Left)
		{
			m_left = false;
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			m_right = false;
		}
		if (event.key.code == sf::Keyboard::Up)
		{
			m_forward = false;
		}
		if (event.key.code == sf::Keyboard::Down)
		{
			m_backward = false;
		}

		if (event.key.code == sf::Keyboard::Escape)
		{
			game.changeState(Game::GameStateName::MAINMENU);
		}

	}

}

//sort algorithm
void PlayState::combSort(std::vector<int>& order, std::vector<double>& dist, int amount)
{
	int gap = amount;
	bool swapped = false;
	while (gap > 1 || swapped)
	{
		//shrink factor 1.3
		gap = (gap * 10) / 13;
		if (gap == 9 || gap == 10) gap = 11;
		if (gap < 1) gap = 1;
		swapped = false;
		for (int i = 0; i < amount - gap; i++)
		{
			int j = i + gap;
			if (dist[i] < dist[j])
			{
				std::swap(dist[i], dist[j]);
				std::swap(order[i], order[j]);
				swapped = true;
			}
		}
	}
}