#pragma once
#include <boost/function_types/result_type.hpp>
#include "MethodInfo.h"

template<typename funcType>
class StaticDelegate
{
public:
	typedef typename boost::function_types::result_type<funcType>::type ReturnType;
	
	//constructs the delegate and validates it at compile time
	template<typename FuncType>
	StaticDelegate(FuncType func) : _func((void *&)func)
	{
		MethodInfo<funcType>::template ValidateFunc<FuncType, true>();
	}

	template<typename ...ArgTypes>
	ReturnType invoke(ArgTypes ...args)
	{
		MethodInfo<funcType>::template ValidateArgs<ArgTypes...>();
		//assign the function to a placeholder pointer with the correct placeholder type
		ReturnType(*pFunc)(ArgTypes...) = (ReturnType(*)(ArgTypes...))_func;
		//if there is no return type, don't return
		if constexpr(std::is_same<ReturnType, void>::value)
		{
			//cast an evil spell to call the function
			pFunc(args...);
		}
		else
		{
			//cast an evil spell to call the function
			return pFunc(args...);
		}
	}

	template<typename TupleType, size_t ...Is>
	ReturnType invokeParamPack(TupleType args, std::index_sequence<Is...>)
	{
		//if there is no return type, don't return
		if constexpr(std::is_same<ReturnType, void>::value)
		{
			//cast an evil spell to call the function
			invoke(std::get<Is>(args)...);
		}
		else
		{
			//cast an evil spell to call the function
			return invoke(std::get<Is>(args)...);
		}
	}

	static void AssociateFunctionPointer(funcType func)
	{
		_associatedPointer = func;
	}

	static StaticDelegate<funcType> MakeStaticDeligate()
	{
		return StaticDelegate((funcType*)_associatedPointer);
	}
private:
	void *  _func;
	static void *_associatedPointer;
};
template<typename funcType>
void *StaticDelegate<funcType>::_associatedPointer;