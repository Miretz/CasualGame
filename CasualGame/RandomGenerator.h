#pragma once
#include <iostream>
#include <random>

class RandomGenerator {
public:
	RandomGenerator() = default;
	virtual ~RandomGenerator(void) = default;

	bool randomChance(float chance);
	float randomFloat(float min, float max);
	int randomInt(int min, int max);

private:
	std::mt19937 gen{ std::random_device{}() };
};