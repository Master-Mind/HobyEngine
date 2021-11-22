#pragma once

template<typename T>
class Singleton
{
public:
	T *operator->()
	{
		static T theOneObj;

		return &theOneObj;
	}
private:
};

#define DECLARE_SINGLETON(TYPE) \
extern Singleton<TYPE> the##TYPE

#define DECLARE_NAMED_SINGLETON(TYPE, NAME) \
extern Singleton<TYPE> the##NAME

#define DEFINE_SINGLETON(TYPE) \
Singleton<TYPE> the##TYPE

#define DEFINE_NAMED_SINGLETON(TYPE, NAME) \
Singleton<TYPE> the##NAME