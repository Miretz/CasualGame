#pragma once

#include <SFML/Graphics.hpp>

class VectorUtils {
public:

	static const sf::Vector2f& normalize(const sf::Vector2f& source) {
		sf::Vector2f result(source.x, source.y);
		float vecLen = length(result);
		if (vecLen != 0) {
			result.x = result.x / vecLen;
			result.y = result.y / vecLen;
		}
		return result;
	}

	static float length(const sf::Vector2f& source)	{
		return sqrt((source.x * source.x) + (source.y * source.y));
	}
};