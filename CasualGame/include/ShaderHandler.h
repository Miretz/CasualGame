#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

class ShaderHandler {
public:
	static sf::Shader* getShader(const std::string& path);

private:

	class ShaderObject {
	public:
		ShaderObject() = default;
		~ShaderObject();
		sf::Shader* getShader() const;
		bool loadShaderFromFile(const std::string& path);
	private:
		sf::Shader* m_shader = nullptr;
	};

	using shadersMap_t = std::unordered_map<std::string, ShaderObject>;
	static shadersMap_t m_shaders;
};
