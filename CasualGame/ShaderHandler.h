#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

class ShaderHandler {
public:
	static sf::Shader* getShader(const std::string& path);

private:

	class ShaderObject {
	public:
		ShaderObject();
		~ShaderObject();
		sf::Shader* getShader() const;
		bool loadShaderFromFile(const std::string& path);
	private:
		sf::Shader* m_shader;
	};

	typedef std::map<std::string, ShaderObject> shadersMap_t;
	static shadersMap_t m_shaders;
};
