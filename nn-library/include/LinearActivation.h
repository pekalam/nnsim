#pragma once
#include "ActivationFunction.h"
#include <limits>
class LinearActivation :
	public ActivationFunction
{
public:
	LinearActivation() = default;
	double upperLimit() const override { return 0; }
	double lowerLimit() const override { return 0; }
	double Call(double x) const override;
	void Call(matrix& x) const override;
	double DerivativeCall(double x) const override;
	matrix DerivativeCall(const matrix& x) const override;
	ActivationFunction* clone() const override { return new LinearActivation(); }
};

