#pragma once
#include <map>
#include <memory>
#include "Cloneable.h"

template<typename T> 
class ObjectFromStringFactory
{
private:
	std::map<std::string, std::unique_ptr<T>> typeMap;
	virtual void initTypeMap() = 0;
protected:
	ObjectFromStringFactory<T>() = default;
	virtual ~ObjectFromStringFactory<T>() = default;
public:
	T* createActivationFunction(const std::string& name);
	void registerActivationFunction(const std::string& name, const T *function);
};

template <typename T>
T* ObjectFromStringFactory<T>::createActivationFunction(const std::string& name)
{
	if (typeMap.size() == 0)
		initTypeMap();
	if (typeMap.count(name) == 0)
		throw std::exception("Type is not registered in ActivationFunctionFactory");
	const Cloneable<T>* cloneable = dynamic_cast<const Cloneable<T>*>(typeMap[name].get());
	return cloneable->clone();
}

template <typename T>
void ObjectFromStringFactory<T>::registerActivationFunction(const std::string& name, const T* function)
{
	const Cloneable<T>* cloneable = dynamic_cast<const Cloneable<T>*>(function);
	if(!cloneable)
		throw std::exception("Registered type is not Cloneable type");
	typeMap.insert_or_assign(name, std::unique_ptr<T>(cloneable->clone()));
}

