#pragma once
#include "CostFunction.h"

class matrix;

class QuadraticCost : public CostFunction
{
public:
	QuadraticCost() = default;
	double Call(const matrix &x, 
		const matrix &t) override; //TODO: czy potrzebne
	double DerivativeCall(const matrix &x, 
		const matrix &t) override;
	matrix mCall(const matrix &x, 
		const matrix &t) override;
	matrix mDerivativeCall(const matrix &x, 
		const matrix &t) override;

	CostFunction* clone() const override { return new QuadraticCost(); }
};

