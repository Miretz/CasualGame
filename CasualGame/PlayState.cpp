#include "PlayState.h"

#define texWidth 64
#define texHeight 64

RandomGenerator PlayState::gen = RandomGenerator();

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

PlayState::PlayState() : m_pos(22.0f, 11.5f), m_dir(-1.0f, 0.0f), m_plane(0.0f, 0.66f)
{
	//texture generator 
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

	//swap texture X/Y
	//only works with square textures
	for (size_t i = 0; i < 8; i++)
		for (size_t x = 0; x < texWidth; x++)
			for (size_t y = 0; y < x; y++)
				std::swap(m_texture[i][texWidth * y + x], m_texture[i][texWidth * x + y]);

	//rendering texture
	m_screenTex.create(GameConfig::windowWidth, GameConfig::windowHeight);
	m_screenSprite.setTexture(m_screenTex);
	m_screenPix = new sf::Uint8[GameConfig::windowWidth * GameConfig::windowHeight * 4];

	//initialize 2d vector
	m_buffer.resize(GameConfig::windowWidth);
	for (int i = 0; i < GameConfig::windowWidth; ++i) m_buffer[i].resize(GameConfig::windowHeight);
}


PlayState::~PlayState()
{
}

void PlayState::update(float ft)
{

	//2d raycaster update
	const int casts = 8; // must be multiple of 8
	for (int x = 0; x < GameConfig::windowWidth; x+=casts)
	{
		
		float cameraX = 2 * x / static_cast<float>(GameConfig::windowWidth) - 1;
		sf::Vector2f rayPos(m_pos.x, m_pos.y);
		sf::Vector2f rayDir(m_dir.x + m_plane.x * cameraX, m_dir.y + m_plane.y * cameraX);
		sf::Vector2i map(static_cast<int>(rayPos.x), static_cast<int>(rayPos.y));
		
		float rayDirYsq = rayDir.y * rayDir.y;
		float rayDirXsq = rayDir.x * rayDir.x;

		sf::Vector2f deltaDist;
		deltaDist.x = sqrt(1 + rayDirYsq / rayDirXsq);
		deltaDist.y = sqrt(1 + rayDirXsq / rayDirYsq);
		
		sf::Vector2f sideDist;
		sf::Vector2i stepDir;

		if (rayDir.x < 0.0f)
		{
			stepDir.x = -1;
			sideDist.x = (rayPos.x - map.x) * deltaDist.x;
		}
		else
		{
			stepDir.x = 1;
			sideDist.x = (map.x + 1.0f - rayPos.x) * deltaDist.x;
		}

		if (rayDir.y < 0.0f)
		{
			stepDir.y = -1;
			sideDist.y = (rayPos.y - map.y) * deltaDist.y;
		}
		else
		{
			stepDir.y = 1;
			sideDist.y = (map.y + 1.0f - rayPos.y) * deltaDist.y;
		}

		bool hit = false;
		int side;

		while (!hit)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				map.x += stepDir.x;
				side = 0;
			}
			else
			{
				sideDist.y += deltaDist.y;
				map.y += stepDir.y;
				side = 1;
			}
			if (m_level[map.x][map.y] > 0) hit = true;
		}

		float perpWallDist;
		if (side == 0)
			perpWallDist = fabs((map.x - rayPos.x + (1 - stepDir.x) / 2) / rayDir.x);
		else
			perpWallDist = fabs((map.y - rayPos.y + (1 - stepDir.y) / 2) / rayDir.y);

		int lineHeight = abs(static_cast<int>(GameConfig::windowHeight / perpWallDist));

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + GameConfig::windowHeight / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + GameConfig::windowHeight / 2;
		if (drawEnd >= GameConfig::windowHeight) drawEnd = GameConfig::windowHeight - 1;

		//texturing calculations
		int texNum = m_level[map.x][map.y] - 1; //1 subtracted from it so that texture 0 can be used!

		float wallX; //where exactly the wall was hit
		if (side == 1)
		{
			wallX = rayPos.x + ((map.y - rayPos.y + (1 - stepDir.y) / 2) / rayDir.y) * rayDir.x;
		}
		else
		{
			wallX = rayPos.y + ((map.x - rayPos.x + (1 - stepDir.x) / 2) / rayDir.x) * rayDir.y;
		}
		wallX -= floor(wallX);

		//x coordinate on the texture
		int texX = static_cast<int>(wallX * texWidth);
		if (side == 0 && rayDir.x > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;

		//darker sides
		for (int y = drawStart; y<drawEnd; y++)
		{
			int d = y * 256 - GameConfig::windowHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
			int texY = ((d * texHeight) / lineHeight) / 256;
			sf::Uint32 color = m_texture[texNum][texHeight * texX + texY];
			
			//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			if (side == 1) color = (color >> 1) & 8355711;
			//fill the rest
			for (int k = 0; k < casts; k++)
			{
				m_buffer[x + k][y] = color;
			}
		}
	}

	// inputs
	m_moveSpeed = ft/1000 * 5.0f; //the constant value is in squares/second
	m_rotSpeed = ft/1000 * 3.0f; //the constant value is in radians/second
	
	// process movement
	if (m_movement[0])
	{
		//both camera direction and camera plane must be rotated
		float oldDirX = m_dir.x;
		m_dir.x = m_dir.x * cos(m_rotSpeed) - m_dir.y * sin(m_rotSpeed);
		m_dir.y = oldDirX * sin(m_rotSpeed) + m_dir.y * cos(m_rotSpeed);
		float oldPlaneX = m_plane.x;
		m_plane.x = m_plane.x * cos(m_rotSpeed) - m_plane.y * sin(m_rotSpeed);
		m_plane.y = oldPlaneX * sin(m_rotSpeed) + m_plane.y * cos(m_rotSpeed);
	}
	if (m_movement[1]) 
	{
		//both camera direction and camera plane must be rotated
		float oldDirX = m_dir.x;
		m_dir.x = m_dir.x * cos(-m_rotSpeed) - m_dir.y * sin(-m_rotSpeed);
		m_dir.y = oldDirX * sin(-m_rotSpeed) + m_dir.y * cos(-m_rotSpeed);
		float oldPlaneX = m_plane.x;
		m_plane.x = m_plane.x * cos(-m_rotSpeed) - m_plane.y * sin(-m_rotSpeed);
		m_plane.y = oldPlaneX * sin(-m_rotSpeed) + m_plane.y * cos(-m_rotSpeed);
	}
	if (m_movement[2])
	{
		if (m_level[int(m_pos.x + m_dir.x * m_moveSpeed)][int(m_pos.y)] == false) m_pos.x += m_dir.x * m_moveSpeed;
		if (m_level[int(m_pos.x)][int(m_pos.y + m_dir.y * m_moveSpeed)] == false) m_pos.y += m_dir.y * m_moveSpeed;
	}
	if (m_movement[3])
	{
		if (m_level[int(m_pos.x - m_dir.x * m_moveSpeed)][int(m_pos.y)] == false) m_pos.x -= m_dir.x * m_moveSpeed;
		if (m_level[int(m_pos.x)][int(m_pos.y - m_dir.y * m_moveSpeed)] == false) m_pos.y -= m_dir.y * m_moveSpeed;
	}

}

void PlayState::draw(sf::RenderWindow & window)
{
	window.clear();
	
	int pixelPos = 0;
	for (int y = 0; y < GameConfig::windowHeight; y++)
	{
		for (int x = 0; x < GameConfig::windowWidth; x++)
		{
			sf::Uint32 color = m_buffer[x][y];
			m_screenPix[pixelPos++] = color & 0x000000ff;
			m_screenPix[pixelPos++] = (color & 0x0000ff00) >> 8;
			m_screenPix[pixelPos++] = (color & 0x00ff0000) >> 16;
			m_screenPix[pixelPos++] = 255;//sf::Uint8((value & 0xff000000) >> 24);
		}
	}
	
	m_screenTex.update(m_screenPix);
	window.draw(m_screenSprite);
	window.display();

	//clear the buffer
	for (int x = 0; x < GameConfig::windowWidth; x++) for (int y = 0; y < GameConfig::windowHeight; y++) m_buffer[x][y] = 0; //clear the buffer instead of cls()
	for (int x = 0; x < (GameConfig::windowHeight * GameConfig::windowWidth * 4); x++) m_screenPix[x] = 0;

}

void PlayState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game)
{

	m_mousePos = mousepPosition;

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed) 
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			game.changeState(Game::GameStateName::MAINMENU);
		}

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
	}
	
}
