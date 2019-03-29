#pragma once
#include <map>
#include <memory>
#include <string>
#include "ObjectFromStringFactory.h"

class ActivationFunction;

class ActivationFunctionFactory : public ObjectFromStringFactory<ActivationFunction>
{
private:
	static std::unique_ptr<ActivationFunctionFactory> instance_;
	ActivationFunctionFactory() = default;
	void initTypeMap() override;
public:
	virtual ~ActivationFunctionFactory() = default;
	static ActivationFunctionFactory& instance()
	{
		if (instance_ == nullptr)
			instance_ = std::unique_ptr<ActivationFunctionFactory>(new ActivationFunctionFactory());
		return *instance_.get();
	}
};

