#pragma once
#include <qmutex.h>

template <typename T>
class ThreadSafe
{
private:
	T data;
	QMutex mutex;
public:
	template<typename T>
	struct isPointer { static const bool value = false; };
	template<typename T>
	struct isPointer<T*> { static const bool value = true; };
	ThreadSafe() = default;
	ThreadSafe(T &data)
	{
		this->data = data;
	}
	~ThreadSafe()
	{
		if (isPointer<T>::value)
			delete data;
	}
	ThreadSafe(T &&data)
	{
		this->data = data;
	}
	void operator+ ()
	{
		mutex.unlock();
	}
	void operator- ()
	{
		mutex.lock();
	}
	T& get()
	{
		return data;
	}
};
