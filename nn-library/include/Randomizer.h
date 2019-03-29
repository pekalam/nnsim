#pragma once
#include <random>

class matrix;

class Randomizer
{
public:
	Randomizer() = default;
	~Randomizer() = default;
	static std::default_random_engine gen;
	static void fromRange(double lowerBound, double upperBound, matrix& x);
	static void fromNormalDist(matrix &x);
};

