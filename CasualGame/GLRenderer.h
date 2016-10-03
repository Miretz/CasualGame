#pragma once

// Link statically with GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "../deps/lib/glew32s.lib") // Link to glew32s.lib

// required for SFML to work with OpenGL
#include <SFML/OpenGL.hpp>

class GLRenderer
{
public:
	GLRenderer();
	virtual ~GLRenderer() = default;
	void init(unsigned char* buffer, int width, int height);
	void cleanup() const;
	void draw(unsigned char* buffer, int width, int height) const;
	void unbindBuffers() const;
	void bindBuffers() const;

private:
	//opengl stuffs
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint tex;
};

