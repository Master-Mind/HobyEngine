#include "StaticConstructor.h"

//std::vector<StaticConstructor *> ClassThatOnlyExistsSoThatTheLinkerDoesntChuckMaShiz::_funcsToCall;

void ClassThatOnlyExistsSoThatTheLinkerDoesntChuckMaShiz::callAll()
{
	//for (auto func : _funcsToCall)
	{
		//func->call();
	}
}

//void ClassThatOnlyExistsSoThatTheLinkerDoesntChuckMaShiz::add(StaticConstructor *f)
//{
//	//_funcsToCall.push_back(f);
//}
