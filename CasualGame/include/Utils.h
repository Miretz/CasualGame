#pragma once

#include <SFML/Graphics.hpp>

class Utils
{
public:
	static const sf::Vector2f normalize(const sf::Vector2f& source);
	static float length(const sf::Vector2f& source);
	static void combSort(std::vector<int>& order, std::vector<double>& dist, int amount);
	static std::string readFile(const std::string path);
};