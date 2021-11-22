#pragma once
#include <vector>
#include "Singleton.h"
typedef void(*FuncICanRun)();
//class StaticConstructor;
class ClassThatOnlyExistsSoThatTheLinkerDoesntChuckMaShiz
{
public:
	static void callAll();
	//static void add(StaticConstructor *f);
private:
	//static std::vector<StaticConstructor *> _funcsToCall;
};

class StaticConstructor
{
public:
	StaticConstructor(FuncICanRun funcToCall) : fuf(funcToCall)
	{
		fuf();
	}

	void call() { fuf(); }
private:
	FuncICanRun fuf;
};

#define STATICCONSTRUCTOR(TYPE) \
void scFunc##TYPE(); \
StaticConstructorsc##TYPE(scFunc##TYPE); \
void scFunc##TYPE()