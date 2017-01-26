#pragma once

#include "GL/glew.h"
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

