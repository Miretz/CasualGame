#pragma once

using GLuint = unsigned int;

class GLRenderer
{
public:
	GLRenderer() = default;
	virtual ~GLRenderer() = default;
	void init(unsigned char* buffer, int width, int height);
	void cleanup() const;
	void draw(unsigned char* buffer, int width, int height) const;
	void unbindBuffers() const;
	void bindBuffers() const;

private:
	//opengl stuffs
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLuint shaderProgram = 0;
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint tex = 0;
};

