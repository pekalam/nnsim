#pragma once
#include "Cloneable.h"
class matrix;

class RandomValueGenerator : public Cloneable<RandomValueGenerator>
{
public:
	virtual void random(matrix& mat) = 0;
	virtual ~RandomValueGenerator() = default;
	virtual RandomValueGenerator* clone() const override = 0;
};

