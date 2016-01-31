#include "PlayState.h"

// Shader sources
const GLchar* vertexSource =
	"#version 150 core\n"
	"in vec2 position;"
	"in vec4 color;"
	"in vec2 texcoord;"
	"out vec4 Color;"
	"out vec2 Texcoord;"
	"void main()"
	"{"
	"    Color = color;"
	"    Texcoord = texcoord;"
	"    gl_Position = vec4(position, 0.0, 1.0);"
	"}";
const GLchar* fragmentSource =
	"#version 150 core\n"
	"in vec4 Color;"
	"in vec2 Texcoord;"
	"out vec4 outColor;"
	"uniform sampler2D tex;"
	"void main()"
	"{"
	"    outColor = texture(tex, Texcoord) * Color;"
	"}";

PlayState::PlayState(const int w, const int h, std::shared_ptr<Player> player, std::shared_ptr<LevelReaderWriter> levelReader) :
	m_windowWidth(w), 
	m_windowHeight(h),
	m_player(move(player)),
	m_levelReader(move(levelReader)),
	m_levelRef(m_levelReader->getLevel()),
	m_spriteRef(m_levelReader->getSprites()),
	m_levelSize(m_levelReader->getLevel().size()),
	m_spriteSize(m_levelReader->getSprites().size()),
	m_mousePosition(sf::Vector2f(0.0f, 0.0f))
{
	
	m_ZBuffer.resize(w);
	m_spriteOrder.resize(m_levelReader->getSprites().size());
	m_spriteDistance.resize(m_levelReader->getSprites().size());

	//FIXME create separate font loader
	m_font.loadFromFile("resources/font/OtherF.ttf");
	m_fpsDisplay.setFont(m_font);
	m_fpsDisplay.setString("fps");
	m_fpsDisplay.setCharacterSize(32);
	m_fpsDisplay.setPosition(float(w) - 10.0f, 0.0f);
	m_fpsDisplay.setColor(sf::Color::Yellow);

	m_buffer = new sf::Uint8[h * w * 3];
	
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Create a Vertex Buffer Object and copy the vertex data to it
	vbo = 0;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
	//   Position     Color                   Texcoords
		-1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
		 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
		 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	ebo = 0;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	// Load texture
	tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

PlayState::~PlayState() {
}

void PlayState::cleanup() {

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteTextures(1, &tex);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);
}

void PlayState::update(const float ft) {

	if (m_forward || m_backward || m_left || m_right) {
		// convert ms to seconds
		double fts = static_cast<double>(ft / 1000);
		double moveSpeed = fts * 5.0; //the constant value is in squares/second
		double rotSpeed = fts * 3.0; //the constant value is in radians/second

		if (m_left) {
			//both camera direction and camera plane must be rotated
			double oldDirX = m_player->m_dirX;
			m_player->m_dirX = m_player->m_dirX * cos(rotSpeed) - m_player->m_dirY * sin(rotSpeed);
			m_player->m_dirY = oldDirX * sin(rotSpeed) + m_player->m_dirY * cos(rotSpeed);
			double oldPlaneX = m_player->m_planeX;
			m_player->m_planeX = m_player->m_planeX * cos(rotSpeed) - m_player->m_planeY * sin(rotSpeed);
			m_player->m_planeY = oldPlaneX * sin(rotSpeed) + m_player->m_planeY * cos(rotSpeed);
		}
		if (m_right) {
			//both camera direction and camera plane must be rotated
			double oldDirX = m_player->m_dirX;
			m_player->m_dirX = m_player->m_dirX * cos(-rotSpeed) - m_player->m_dirY * sin(-rotSpeed);
			m_player->m_dirY = oldDirX * sin(-rotSpeed) + m_player->m_dirY * cos(-rotSpeed);
			double oldPlaneX = m_player->m_planeX;
			m_player->m_planeX = m_player->m_planeX * cos(-rotSpeed) - m_player->m_planeY * sin(-rotSpeed);
			m_player->m_planeY = oldPlaneX * sin(-rotSpeed) + m_player->m_planeY * cos(-rotSpeed);
		}
		if (m_forward) {
			if (m_levelRef[int(m_player->m_posX + m_player->m_dirX * moveSpeed)][int(m_player->m_posY)] == false) m_player->m_posX += m_player->m_dirX * moveSpeed;
			if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY + m_player->m_dirY * moveSpeed)] == false) m_player->m_posY += m_player->m_dirY * moveSpeed;
		}
		if (m_backward) {
			if (m_levelRef[int(m_player->m_posX - m_player->m_dirX * moveSpeed)][int(m_player->m_posY)] == false) m_player->m_posX -= m_player->m_dirX * moveSpeed;
			if (m_levelRef[int(m_player->m_posX)][int(m_player->m_posY - m_player->m_dirY * moveSpeed)] == false) m_player->m_posY -= m_player->m_dirY * moveSpeed;
		}
	}
}

void PlayState::draw(sf::RenderWindow& window) {

	//clear the buffer
	const int bufsize = m_windowHeight * m_windowWidth * 3;
	for (int i = bufsize; i-- >= 0;) {
		m_buffer[i] = 0;
	}

	//calculate a new buffer
	calculateWalls();
	calculateSprites();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_windowWidth, m_windowHeight, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	window.pushGLStates();

	//draw minimap
	drawMinimap(&window);

	//draw fps display
	window.draw(m_fpsDisplay);

	window.popGLStates();

	// Create Vertex Array Object
	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindTexture(GL_TEXTURE_2D, tex);

	window.display();
}

void PlayState::calculateWalls() {

	const double rayPosX = m_player->m_posX;
	const double rayPosY = m_player->m_posY;

	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;

	//length of ray from current position to next x or y-side
	double sideDistX;
	double sideDistY;

	double perpWallDist;
	double wallX; //where exactly the wall was hit

	const std::vector<sf::Uint32>& tex8 = m_levelReader->getTexture(8);//floor
	const std::vector<sf::Uint32>& tex9 = m_levelReader->getTexture(9);//ceiling

	//2d raycaster
	for (int x = 0; x < m_windowWidth; x++)	{
		//calculate ray position and direction
		const double cameraX = 2 * x / double(m_windowWidth) - 1; //x-coordinate in camera space
		
		const double rayDirX = m_player->m_dirX + m_player->m_planeX * cameraX;
		const double rayDirY = m_player->m_dirY + m_player->m_planeY * cameraX;

		//which box of the map we're in
		int mapX = static_cast<int>(rayPosX);
		int mapY = static_cast<int>(rayPosY);

		//length of ray from one x or y-side to next x or y-side
		const double rayDirXsq = rayDirX * rayDirX;
		const double rayDirYsq = rayDirY * rayDirY;
		const double deltaDistX = sqrt(1 + rayDirYsq / rayDirXsq);
		const double deltaDistY = sqrt(1 + rayDirXsq / rayDirYsq);

		//calculate step and initial sideDist
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (rayPosX - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
		}
		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (rayPosY - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
		}

		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?

		//perform DDA
		while (hit == 0) {
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (m_levelRef[mapX][mapY] > 0) hit = 1;
		}

		//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
		if (side == 0) {
			perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
		}
		else {
			perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);
		}

		//Calculate height of line to draw on screen
		const int lineHeight = static_cast<int>(std::abs(m_windowHeight / perpWallDist));

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + m_windowHeight / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + m_windowHeight / 2;
		if (drawEnd >= m_windowHeight)drawEnd = m_windowHeight - 1;

		//texturing calculations
		const int texNum = m_levelRef[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

		if (side == 1) {
			wallX = rayPosX + ((mapY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
		}
		else {
			wallX = rayPosY + ((mapX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
		}
		wallX -= floor(wallX);

		//x coordinate on the texture
		int texX = static_cast<int>(wallX * texWidth);
		if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
		if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

		const std::vector<sf::Uint32>& texture = m_levelReader->getTexture(texNum);
		const size_t texSize = texture.size();

		//darker sides
		for (int y = drawStart; y < drawEnd; y++) {

			const int d = y * 256 - m_windowHeight * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
			const int texY = ((d * texHeight) / lineHeight) / 256;
			const unsigned int texNumY = texHeight * texX + texY;

			if (texNumY < texSize) {

				sf::Uint32 color = texture[texNumY];

				setPixel(x, y, color, side==1);

			}
		}

		//SET THE ZBUFFER FOR THE SPRITE CASTING
		m_ZBuffer[x] = perpWallDist; //perpendicular distance is used

		//FLOOR CASTING
		double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

		//4 different wall directions possible
		if (side == 0 && rayDirX > 0) {
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		}
		else if (side == 0 && rayDirX < 0) {
			floorXWall = mapX + 1.0;
			floorYWall = mapY + wallX;
		}
		else if (side == 1 && rayDirY > 0) {
			floorXWall = mapX + wallX;
			floorYWall = mapY;
		}
		else {
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0;
		}

		if (drawEnd < 0) drawEnd = m_windowHeight; //becomes < 0 when the integer overflows
		
		//draw the floor from drawEnd to the bottom of the screen
		for (int y = drawEnd + 1; y < m_windowHeight; y++) {

			const double currentDist = m_windowHeight / (2.0 * y - m_windowHeight); //you could make a small lookup table for this instead
			const double weight = currentDist / perpWallDist;

			const double currentFloorX = weight * floorXWall + (1.0 - weight) * m_player->m_posX;
			const double currentFloorY = weight * floorYWall + (1.0 - weight) * m_player->m_posY;

			const int floorTexX = int(currentFloorX * texWidth) % texWidth;
			const int floorTexY = int(currentFloorY * texHeight) % texHeight;

			//floor textures
			sf::Uint32 color1 = tex8[texWidth * floorTexY + floorTexX];
			sf::Uint32 color2 = tex9[texWidth * floorTexY + floorTexX];

			setPixel(x, y, color1, false);
			setPixel(x, m_windowHeight - y, color2, false);
		}
	}

}

void PlayState::calculateSprites() {

	//SPRITE CASTING
	//sort sprites from far to close
	for (size_t i = 0; i < m_spriteSize; i++) {
		m_spriteOrder[i] = i;
		m_spriteDistance[i] = ((m_player->m_posX - m_spriteRef[i].x) * (m_player->m_posX - m_spriteRef[i].x) + (m_player->m_posY - m_spriteRef[i].y) * (m_player->m_posY - m_spriteRef[i].y)); //sqrt not taken, unneeded
	}
	combSort(m_spriteOrder, m_spriteDistance, m_spriteRef.size());

	//after sorting the sprites, do the projection and draw them
	for (size_t i = 0; i < m_spriteRef.size(); i++) {
		
		//translate sprite position to relative to camera
		const double spriteX = m_spriteRef[m_spriteOrder[i]].x - m_player->m_posX;
		const double spriteY = m_spriteRef[m_spriteOrder[i]].y - m_player->m_posY;

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		const double invDet = 1.0 / (m_player->m_planeX * m_player->m_dirY - m_player->m_dirX * m_player->m_planeY); //required for correct matrix multiplication
		const double transformX = invDet * (m_player->m_dirY * spriteX - m_player->m_dirX * spriteY);
		const double transformY = invDet * (-m_player->m_planeY * spriteX + m_player->m_planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D       
		const int spriteScreenX = int((m_windowWidth / 2) * (1 + transformX / transformY));

		//calculate height of the sprite on screen
		const int spriteHeight = abs(int(m_windowHeight / (transformY))); //using "transformY" instead of the real distance prevents fisheye

		//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -spriteHeight / 2 + m_windowHeight / 2;
		if (drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + m_windowHeight / 2;
		if (drawEndY >= m_windowHeight) drawEndY = m_windowHeight - 1;

		//calculate width of the sprite
		int spriteWidth = spriteHeight;
		int drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0) drawStartX = 0;
		int drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= m_windowWidth) drawEndX = m_windowWidth - 1;

		const int texNr = m_spriteRef[m_spriteOrder[i]].texture;
		const std::vector<sf::Uint32>& textureData = m_levelReader->getTexture(texNr);
		const int texSize = textureData.size();

		//check mouse over and highlight
		bool mouseOver = false;
		if (texNr != 12) {
			//half width
			const float wOutline = float(drawEndX - drawStartX) / 2.0f;
			const float hOutline = float(drawEndY - drawStartY);
			Clickable outline(sf::Vector2f(wOutline, hOutline), sf::Vector2f(drawStartX + wOutline / 2.0f, float(drawStartY)));
			if (outline.isMouseOver(m_mousePosition)) {
				mouseOver = true;
			}
		}

		//loop through every vertical stripe of the sprite on screen
		for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
			
			const int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
			
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if (transformY > 0 && stripe > 0 && stripe < m_windowWidth && transformY < m_ZBuffer[stripe]) {
				
				//for every pixel of the current stripe
				for (int y = drawStartY; y < drawEndY; y++) {
					
					const int d = (y)* 256 - m_windowHeight * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
					const int texY = ((d * texHeight) / spriteHeight) / 256;
					const int texPix = texWidth * texX + texY;
					
					// prevent exception when accessing tex pixel out of range
					if (texPix < texSize) {
						sf::Uint32 color = textureData[texPix]; //get current color from the texture
						
						// black is invisible!!!
						if ((color & 0x00FFFFFF) != 0) {
							//brighten if mouse over
							setPixel(stripe, y, color, mouseOver ? HIGHLIGHT : 0);
						}
					}
				}

			}
		}
	}
}

//Render minimap
void PlayState::drawMinimap(sf::RenderWindow* window) {

	//minimap background
	sf::RectangleShape minimapBg(sf::Vector2f(m_levelSize * minimapScale, m_levelSize * minimapScale));
	minimapBg.setPosition(0, 0);
	minimapBg.setFillColor(sf::Color(150, 150, 150, minimapTransparency));
	window->draw(minimapBg);

	//draw walls
	for (size_t y = 0; y < m_levelSize; y++) {
		for (size_t x = 0; x < m_levelSize; x++) {
			if (m_levelRef[y][x] > 0 && m_levelRef[y][x] < 9) {
				sf::RectangleShape wall(sf::Vector2f(minimapScale, minimapScale));
				wall.setPosition(x * minimapScale, y * minimapScale);
				wall.setFillColor(sf::Color(0, 0, 0, minimapTransparency));
				window->draw(wall);
			}
		}
	}

	// Render entities on minimap
	for (size_t i = 0; i < m_spriteSize; i++) {
		sf::CircleShape object(minimapScale / 4.0f);
		object.setPosition(float(m_spriteRef[i].y) * minimapScale, float(m_spriteRef[i].x) * minimapScale);
		object.setOrigin(minimapScale / 2.0f, minimapScale / 2.0f);
		object.setFillColor(sf::Color(0, 0, 255, minimapTransparency));
		window->draw(object);
	}
	
	// Render Player on minimap
	{
		sf::CircleShape player(float(minimapScale), 3);
		player.setPosition(float(m_player->m_posY) * minimapScale, float(m_player->m_posX) * minimapScale);
		player.setFillColor(sf::Color(255, 255, 255, minimapTransparency));
		player.setOrigin(float(minimapScale), float(minimapScale));

		sf::RectangleShape player2(sf::Vector2f(minimapScale / 2.0f, minimapScale / 2.0f));
		player2.setPosition(float(m_player->m_posY) * minimapScale, float(m_player->m_posX) * minimapScale);
		player2.setFillColor(sf::Color(255, 255, 255, minimapTransparency));
		player2.setOrigin(minimapScale / 4.0f, -minimapScale / 2.0f);

		float angle = std::atan2f(float(m_player->m_dirX), float(m_player->m_dirY));
		player.setRotation((angle * 57.2957795f) + 90);
		player2.setRotation((angle * 57.2957795f) + 90);

		window->draw(player);
		window->draw(player2);
	}
}

void PlayState::setPixel(int x, int y, const sf::Uint32 colorRgba, int style){

	if (x >= m_windowWidth || y >= m_windowHeight) {
		return;
	}

	sf::Uint8 *colors = (sf::Uint8*)&colorRgba;
	int index = (y * m_windowWidth + x) * 3;

	if (style == DARKEN) {
		m_buffer[index] = colors[0] / 2;
		m_buffer[index + 1] = colors[1] / 2;
		m_buffer[index + 2] = colors[2] / 2;
	}
	else if (style == HIGHLIGHT) {
		m_buffer[index] = std::min(colors[0] + 25, 255);
		m_buffer[index + 1] = std::min(colors[1] + 25, 255);
		m_buffer[index + 2] = std::min(colors[2] + 25, 255);
	}
	else {
		m_buffer[index] = colors[0];
		m_buffer[index + 1] = colors[1];
		m_buffer[index + 2] = colors[2];
	}
}

void PlayState::handleInput(const sf::Event & event, const sf::Vector2f & mousepPosition, Game & game) {

	m_mousePosition = mousepPosition;

	//update fps from game
	m_fpsDisplay.setString(std::to_string(game.getFps()));
	m_fpsDisplay.setOrigin(m_fpsDisplay.getGlobalBounds().width, 0.0f);

	if (event.type == sf::Event::MouseButtonPressed) {
		m_wasMouseClicked = true;
	}

	//escape go to main menu
	if (event.type == sf::Event::KeyPressed) {
		// handle controls
		if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A)) {
			m_left = true;
		}
		else if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D)) {
			m_right = true;
		}
		else if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W)) {
			m_forward = true;
		}
		else if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S))	{
			m_backward = true;
		}

	}

	if (event.type == sf::Event::KeyReleased) {
		// handle controls
		if ((event.key.code == sf::Keyboard::Left) || (event.key.code == sf::Keyboard::A)) {
			m_left = false;
		}
		if ((event.key.code == sf::Keyboard::Right) || (event.key.code == sf::Keyboard::D)) {
			m_right = false;
		}
		if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::W)) {
			m_forward = false;
		}
		if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::S)) {
			m_backward = false;
		}
		if (event.key.code == sf::Keyboard::Escape)	{
			cleanup();
			game.changeState(Game::GameStateName::MAINMENU);
		}
	}
}

//sort algorithm
void PlayState::combSort(std::vector<int>& order, std::vector<double>& dist, int amount) {
	
	int gap = amount;
	bool swapped = false;
	while (gap > 1 || swapped) {
		//shrink factor 1.3
		gap = (gap * 10) / 13;
		if (gap == 9 || gap == 10) gap = 11;
		if (gap < 1) gap = 1;
		swapped = false;
		for (int i = 0; i < amount - gap; i++) {
			int j = i + gap;
			if (dist[i] < dist[j]) {
				std::swap(dist[i], dist[j]);
				std::swap(order[i], order[j]);
				swapped = true;
			}
		}
	}
}