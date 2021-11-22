#pragma once
#include <boost/function_types/result_type.hpp>
#include "MethodInfo.h"
//a placeholder that allows delegate to generically call a member function of any class
class tempClass
{
public:
	template<typename Ret, typename ...ArgTypes>
	Ret tempCall(ArgTypes ...args)
	{
		return Ret();
	}
};

//a member function and an object
template<typename funcType>
class Delegate
{
public:
	typedef typename boost::function_types::result_type<funcType>::type ReturnType;
  //constructs the delegate and validates it at compile time
	template<typename ClassType, typename FuncType>
	Delegate(ClassType *obj, FuncType func) : _obj(obj), _func((void *&)func)
	{
		MethodInfo<funcType>::ValidateFunc<FuncType, !std::is_member_function_pointer<FuncType>::value>();
	}

	template<typename ObjType, typename TupleType>
	static ReturnType invokeOnObj(ObjType *obj, TupleType params)
	{
		Delegate<funcType> del(obj, (funcType*)Funcs()[TYPE_ID(ObjType)]);

		if constexpr(std::is_same<ReturnType, void>::value)
		{
			del.invokeParamPack(params, std::make_index_sequence<std::tuple_size<TupleType>::value>());
		}
		else
		{
			return del.invokeParamPack(params, std::make_index_sequence<std::tuple_size<TupleType>::value>{});
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
	template<typename ...ArgTypes>
	ReturnType invoke(ArgTypes ...args)
	{
		MethodInfo<funcType>::ValidateArgs<ArgTypes...>();
    //assign the function to a placeholder pointer with the correct placeholder type
		ReturnType(__thiscall tempClass::* pFunc)(ArgTypes...) = (ReturnType(__thiscall tempClass::*& )(ArgTypes...))_func;
    //if there is no return type, don't return
		if constexpr(std::is_same<ReturnType,void>::value)
		{
       //cast an evil spell to call the function
			(reinterpret_cast<tempClass*>(_obj)->*pFunc)(args...);
		}
		else
		{
       //cast an evil spell to call the function
			return (reinterpret_cast<tempClass*>(_obj)->*pFunc)(args...);
		}
	}
  //used by the event system when removing a listener
	bool HasObj(const void *obj) const
	{
		return _obj == obj;
	}

	template<typename ObjType>
	void AddFunc()
	{
		Funcs().insert({ TYPE_ID(ObjType) ,_func });
	}

private:
	static std::unordered_map<size_t, void *>  &Funcs()
	{
		static std::unordered_map<size_t, void *> ret;

		return ret;
	}

	void *  _func;
	void *_obj;
};



template <typename R, typename ClassType, typename... T>
static Delegate<R(T...)> *makeDelegate(R(__thiscall ClassType::*func)(T...))
{
	return new Delegate<R (T...)>(static_cast<ClassType*>(nullptr), func);
}
