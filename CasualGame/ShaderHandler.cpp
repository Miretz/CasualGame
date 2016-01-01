#include "ShaderHandler.h"

ShaderHandler::ShadersMap ShaderHandler::m_shaders;

sf::Shader* ShaderHandler::getShader(const std::string& path)
{
	ShadersMap::iterator it = m_shaders.find(path);

	if (it != m_shaders.end())
		return it->second.getShader();

	if (!m_shaders[path].loadShaderFromFile(path))
		std::cout << "An error occured while loading the following shader:" << path << std::endl;

	return m_shaders[path].getShader();
}

ShaderHandler::ShaderObject::ShaderObject()
{
	m_shader = NULL;
}

ShaderHandler::ShaderObject::~ShaderObject()
{
	if (m_shader)
		delete m_shader;
}

sf::Shader* ShaderHandler::ShaderObject::getShader() const
{
	return m_shader;
};

bool ShaderHandler::ShaderObject::loadShaderFromFile(const std::string& path)
{
	if (!m_shader)
		m_shader = new sf::Shader;

	return m_shader->loadFromFile(path, sf::Shader::Fragment);
}
