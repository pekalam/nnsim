#pragma once
#include <memory>
#include "ObjectFromStringFactory.h"
#include "RandomValueGenerator.h"

class RandomValueGeneratorFactory : public ObjectFromStringFactory<RandomValueGenerator>
{
private:
	static std::unique_ptr<RandomValueGeneratorFactory> instance_;
	RandomValueGeneratorFactory() = default;
	void initTypeMap() override;
public:
	static RandomValueGeneratorFactory& instance()
	{
		if (instance_ == nullptr)
			instance_ = std::unique_ptr<RandomValueGeneratorFactory>(new RandomValueGeneratorFactory());
		return *instance_;
	}
	~RandomValueGeneratorFactory() = default;
};

