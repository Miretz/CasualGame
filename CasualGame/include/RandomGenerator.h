#pragma once

#include <random>

class RandomGenerator
{
public:
	bool randomChance(float chance);
	float randomFloat(float min, float max);
	int randomInt(int min, int max);

private:
	std::mt19937 gen{ std::random_device{}() };
};