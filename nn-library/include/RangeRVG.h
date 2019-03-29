#pragma once
#include "RandomValueGenerator.h"
#include <random>

class RangeRVG :
	public RandomValueGenerator
{
private:
	std::default_random_engine gen;
	std::uniform_real_distribution<double> dst;
	double lowerBound;
	double upperBound;
public:
	void random(matrix& mat) override;
	RangeRVG(double lowerBound, double upperBound);
	virtual ~RangeRVG();
	RandomValueGenerator* clone() const override { return new RangeRVG(lowerBound, upperBound); }
};

