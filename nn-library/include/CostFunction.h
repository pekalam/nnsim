#pragma once
#include "Cloneable.h"

class matrix;

class CostFunction : public Cloneable<CostFunction>
{
public:
	virtual double Call(const matrix &x, 
		const matrix &t) = 0;
	virtual double DerivativeCall(const matrix &x,
		const matrix &t) = 0;
	virtual matrix mCall(const matrix &x,
		const matrix &t) = 0;
	virtual matrix mDerivativeCall(const matrix &x,
		const matrix &t) = 0;
	virtual CostFunction* clone() const override = 0;
	virtual ~CostFunction() = default;
};

