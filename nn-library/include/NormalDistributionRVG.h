#pragma once
#include "RandomValueGenerator.h"
#include <random>

class NormalDistributionRVG :
	public RandomValueGenerator
{
private:
	std::default_random_engine gen;
	std::normal_distribution<double> dst;
public:
	void random(matrix& mat) override;
	NormalDistributionRVG() = default;
	virtual ~NormalDistributionRVG() = default;
	RandomValueGenerator* clone() const override { return new NormalDistributionRVG(); }
};

