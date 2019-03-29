#pragma once
#include "ObjectFromStringFactory.h"
#include "CostFunction.h"

class CostFunctionFactory : public ObjectFromStringFactory<CostFunction>
{
private:
	static std::unique_ptr<CostFunctionFactory> instance_;
	CostFunctionFactory() = default;
	void initTypeMap() override;
public:
	static CostFunctionFactory& instance()
	{
		if (instance_ == nullptr)
			instance_ = std::unique_ptr<CostFunctionFactory>(new CostFunctionFactory());
		return *instance_.get();
	}
};

