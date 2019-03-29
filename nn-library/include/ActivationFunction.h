#pragma once
#include "Cloneable.h"
class matrix;

class ActivationFunction : public Cloneable<ActivationFunction>
{
public:
	virtual	double Call(double x) const = 0;
	virtual	void Call(matrix &x) const = 0;
	virtual double DerivativeCall(double x) const = 0;
	virtual matrix DerivativeCall(const matrix &x) const = 0;
	virtual double upperLimit() const = 0;
	virtual double lowerLimit() const = 0;
	virtual ActivationFunction* clone() const override = 0;
	virtual ~ActivationFunction() = default;
};

