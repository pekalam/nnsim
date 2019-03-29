#pragma once
#include "ActivationFunction.h"
class Sigmoid :
	public ActivationFunction
{
public:
	double upperLimit() const override { return 1; }
	double lowerLimit() const override { return 0; }
	double Call(double x) const override;
	void Call(matrix& x) const override;
	double DerivativeCall(double x)const override;
	matrix DerivativeCall(const matrix& x) const override;
	Sigmoid() = default;
	ActivationFunction* clone() const override { return new Sigmoid(); }
};

