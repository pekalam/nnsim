#pragma once
#include "ActivationFunction.h"
class TanH :
	public ActivationFunction
{
public:
	ActivationFunction* clone() const override { return new TanH(); }
	TanH() = default;
	double upperLimit() const override { return 1; }
	double lowerLimit() const override { return -1; }
	double Call(double x) const override;
	void Call(matrix& x) const override;
	double DerivativeCall(double x) const override;
	matrix DerivativeCall(const matrix& x) const override;
};

