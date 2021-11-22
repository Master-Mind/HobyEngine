#pragma once
#include <tuple>
#include <functional>
#include <stdarg.h>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/function_types/result_type.hpp>
#include "MetaReference.h"
//#include "MetaDB.h"
//
//struct MethodInterface
//{
//public:
//	virtual ~MethodInterface() = default;
//	virtual MetaReference GetReturnType() const = 0;
//	virtual MetaReference GetArgumentType(int argIndex)const = 0;
//};
template<typename funcType>
struct MethodInfo// : public MethodInterface
{
public:
	static const size_t NumArgs = boost::function_types::function_arity<funcType>::value;
  //meta data for each of the argument types
	//static MetaReference ArgumentTypes[];
	typedef typename boost::function_types::result_type<funcType>::type ReturnType;
	typedef MethodInfo<funcType> myType;
	MethodInfo()
	{
		
	}
	//MetaReference &GetReturnType()const
	//{
	//	return GetMetaData<ReturnType>();
	//}
  
  //verify that the function that is given in FuncType is the same as this method info's type (called in delegate())
	template<typename FuncType, const bool isStatic>
	static void ValidateFunc()
	{
    //check that the return and number of arguments is the same
		static_assert(std::is_same<typename boost::function_types::result_type<FuncType>::type, ReturnType>::value, "Error: the return type of the given function was wrong");
		if constexpr(isStatic)
		{
			static_assert(boost::function_types::function_arity<FuncType>::value == NumArgs, "Error: the number of arguments was wrong");
		}
		else
		{
			static_assert(boost::function_types::function_arity<FuncType>::value == NumArgs + 1, "Error: the number of arguments was wrong");
		}
    //recursively verify each of the arguments
		ValidateFuncArg<0, FuncType, isStatic>();

	}
	template<int i, typename FuncType, const bool isStatic>
	static void ValidateFuncArg()
	{
    //if the arg takes a pointer to a dirived class, and the dirived class is dirived from the proper argument, then
    //it is fine
		IsSameEnough<typename boost::mpl::at_c<boost::function_types::parameter_types<funcType>, i>::type,
		typename boost::mpl::at_c<boost::function_types::parameter_types<FuncType>, i + !isStatic>::type>();
		if constexpr(i < NumArgs)
		{
			ValidateFuncArg<i + 1, FuncType, isStatic>();
		}
	}
	template<typename ...ArgTypes>
	static void ValidateArgs()
	{
		static_assert(sizeof...(ArgTypes) == NumArgs, "Error: A delegate was not given the proper number of arguments");
		ValidateArg<0, ArgTypes...>(); //Recursively validate each argument type and run time
	}

	template<typename lhs, typename rhs>
	static void IsSameEnough()
	{
		const bool IsSame = std::is_same<lhs, rhs>::value;
		if constexpr (!IsSame)
		{
			if constexpr(std::is_pointer<lhs>::value)
			{
				static_assert(std::is_base_of<typename std::remove_pointer<lhs>::type, typename std::remove_pointer<rhs>::type>::value, "One of the provided types to this delegate was invalid");
			}
			else
			{
				static_assert(IsSame, "One of the provided types to this delegate was invalid");
			}
		}
	}

	template <typename R, typename... T>
	static std::tuple<T...> function_args(R(*)(T...))
	{
		return std::tuple<T...>();
	}

	static auto MakeBlankParamTuple()
	{
		funcType *tempDontUse = nullptr;
		return function_args(tempDontUse);
	}

	template<int i, typename First, typename ...ArgTypes>
	static void ValidateArg()
	{
		IsSameEnough<First, typename boost::mpl::at_c<boost::function_types::parameter_types<funcType>, i>::type>();
		
		ValidateArg<i + 1, ArgTypes...>();
	}
	template<int i>
	static void ValidateArg()
	{
		
	}
	MetaReference GetArgumentType(int argIndex)const
	{
		//return boost::function_types::parameter_types<funcType>::front;
		return MetaReference();
	}
private:
};


//template<typename funcType>
//MetaReference MethodInfo<funcType>::ArgumentTypes[boost::function_types::function_arity<funcType>::value] = {};