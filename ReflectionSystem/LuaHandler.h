#pragma once
//#include <lauxlib.h>
#include "sbool.h"
#include "StaticDelegate.h"
#include "Delegate.h"
#include <tuple>
#include <unordered_set>

#include <vector>
extern "C"
{
//#include <lua.h>
//#include <lauxlib.h>
}
//typedef std::vector<luaL_Reg> LuaRegistry;
struct lua_State;
class MetaInterface;
template <typename T>
class MetaEnum;

class LuaHandler
{
public:
	static void Initialize();
	static void StartUp();
	static void TestLua();
	static int TheLuaFunc(lua_State *L);
	static int LuaPanic(lua_State *L);

	class ClassToRegister
	{
	public:
		const MetaInterface *ref;
		//LuaRegistry dynamicReg;
		//LuaRegistry staticReg;
		//LuaRegistry operators;
	};

	template<typename objType>
	static int LuaIndexer(lua_State *L)
	{
		//auto fuf = LuaIndexGettors.at(TYPE_ID(objType)).at(luaL_checkstring(L));
		return 0;
	}

	template<typename objType, typename ...ConArgs>
	static int LuaConstructor(lua_State *L)
	{
		//std::tuple<ConArgs...> params = MakeFilledParamPack<0, decltype(params)>(L, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{});
		//
		//LuaPushValToStack(L, CallConOnParamPack(params, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{}));

		return 1;
	}

	template<typename objType, typename TupleType, size_t ...Is>
	static inline TupleType CallConOnParamPack(TupleType &params, std::index_sequence<Is...>)
	{
		//return new objType(std::get<Is>(params)...);
	}


	template<typename objType, typename memberType, size_t memberNumber>
	static int LuaGetter(lua_State *L)
	{
		//const size_t offset = MetaDB::getTheFuckingOffsetForFuckingLuaBecauseFuckingHeaderFiles(TYPE_ID(objType), memberNumber);
		//objType *obj = *(objType **)lua_touserdata(L, 1);
		//
		//LuaPushValToStack(L,*reinterpret_cast<memberType*>(reinterpret_cast<char*>(obj) + offset));
		//
		return 1;
	}

	template<typename objType, typename memberType, size_t memberNumber>
	static int LuaSetter(lua_State *L)
	{
		//const size_t offset = MetaDB::getTheFuckingOffsetForFuckingLuaBecauseFuckingHeaderFiles(TYPE_ID(objType), memberNumber);
		//objType *obj = *(objType **)lua_touserdata(L, 1);
		//
		//*reinterpret_cast<memberType*>(reinterpret_cast<char*>(obj) + offset) = getLuaValFromStack<memberType>(L, 2);

		return 1;
	}

	template<typename objType, typename funcType>
	static int LuaMemberFuncWrapper(lua_State *L)
	{
		//auto params = MethodInfo<funcType>::MakeBlankParamTuple();
		//int retNum;
		//
		//objType *obj = *(objType **)lua_touserdata(L, 1);
		//
		//params = MakeFilledParamPack<1, decltype(params)>(L, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{});
		//
		//DEBUGASSERT(lua_gettop(L) - 1 == std::tuple_size<decltype(params)>::value, DTag::Lua);
		//
		//if constexpr(std::is_same<typename MethodInfo<funcType>::ReturnType, void>::value)
		//{
		//	Delegate<funcType>::invokeOnObj(obj, params);
		//	retNum = 0;
		//}
		//else
		//{
		//	LuaPushValToStack(L, Delegate<funcType>::invokeOnObj(obj, params));
		//	retNum = 1;
		//}

		return retNum;
	}

	template<typename funcType>
	static int LuaFuncWrapper(lua_State *L)
	{
		//auto params = MethodInfo<funcType>::MakeBlankParamTuple();
		//
		//int retNum;
		//params = MakeFilledParamPack<0, decltype(params)>(L, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{});
		//
		//DEBUGASSERT(lua_gettop(L) == std::tuple_size<decltype(params)>::value, DTag::Lua);
		//if constexpr(std::is_same<typename MethodInfo<funcType>::ReturnType, void>::value)
		//{
		//	StaticDelegate<funcType>::MakeStaticDeligate().invokeParamPack(params, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{});
		//	retNum = 0;
		//}
		//else
		//{
		//	LuaPushValToStack(L, StaticDelegate<funcType>::MakeStaticDeligate().invokeParamPack(params, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{}));
		//	retNum = 1;
		//}

		return retNum;
	}

	template<typename funcType>
	static int LuaDebugFuncWrapper(lua_State *L)
	{
//		auto params = MethodInfo<funcType>::MakeBlankParamTuple();
//#define TUPSIZE std::tuple_size<decltype(params)>::value
//		if constexpr(std::is_same<typename std::tuple_element<TUPSIZE - 1, decltype(params)>::type, DTag::DebugTag>::value)
//		{
//			int retNum;
//			lua_Debug ar;
//			lua_getstack(L, 1, &ar);
//			lua_getinfo(L, "nSl", &ar);
//			FillParamPackElems<0, std::tuple_size<decltype(params)>::value - 3>(params, L);
//			std::get<TUPSIZE - 1>(params) = (DTag::DebugTag)static_cast<typename std::underlying_type<DTag::DebugTag>::type>(luaL_checkinteger(L, TUPSIZE - 2));
//			std::get<TUPSIZE - 2>(params) = ar.currentline;
//			std::get<TUPSIZE - 3>(params) = ar.source;
//
//			DEBUGASSERT(lua_gettop(L) == TUPSIZE - 2, DTag::Lua);
//
//			if constexpr(std::is_member_function_pointer<funcType>::value)
//			{
//
//			}
//			else
//			{
//				if constexpr(std::is_same<typename MethodInfo<funcType>::ReturnType, void>::value)
//				{
//					StaticDelegate<funcType>::MakeStaticDeligate().invokeParamPack(params, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{});
//					retNum = 0;
//				}
//				else
//				{
//					LuaPushValToStack(L, StaticDelegate<funcType>::MakeStaticDeligate().invokeParamPack(params, std::make_index_sequence<std::tuple_size<decltype(params)>::value>{}));
//					retNum = 1;
//				}
//			}
//
//			return retNum;
//		}
//		else
		{
			return 0;
		}
	}
	template<const int stackOffset, typename TupleType, size_t ...Is>
	static inline TupleType MakeFilledParamPack(lua_State *L, std::index_sequence<Is...>)
	{
		//return TupleType( getLuaValFromStack<typename std::tuple_element<Is,TupleType>::type>(L, Is + 1 + stackOffset)...);
	}

	static void BindClass(ClassToRegister &&class_);


	template<size_t i, size_t count, typename TupleType>
	static inline void FillParamPackElems(TupleType &tup, lua_State *L)
	{
		//if constexpr(i < count)
		//{
		//	std::get<i>(tup) = getLuaValFromStack<typename std::tuple_element<i, TupleType>::type>(L, i + 1);
		//	FillParamPackElems<i + 1, count>(tup, L);
		//}
	}
	template<typename T>
	static inline void LuaPushValToStack(lua_State *L, T val)
	{
		//if constexpr(std::is_integral<T>::value || std::is_enum<T>::value)
		//	lua_pushinteger(L, val);
		//else if constexpr(std::is_pointer<T>::value)
		//{
		//	T*newData = (T*)lua_newuserdata(L, sizeof(T));
		//	*newData = val;
		//	luaL_getmetatable(L, (std::string("luaL_") + getTableName(TYPE_ID(std::remove_pointer<T>::type))).c_str());
		//	lua_setmetatable(L, -2);
		//}
		//else
		//{
		//	T**newData = (T**)lua_newuserdata(L, sizeof(T*));
		//	*newData = &val;
		//	luaL_getmetatable(L, (std::string("luaL_") + getTableName(TYPE_ID(std::remove_pointer<T>::type))).c_str());
		//	lua_setmetatable(L, -2);
		//}
	}

	template<>
	static inline void LuaPushValToStack<float>(lua_State *L, float val)
	{
		//lua_pushnumber(L, val);
	}

	template<>
	static inline void LuaPushValToStack<double>(lua_State *L, double val)
	{
		//lua_pushnumber(L, (lua_Number)val);
	}

	template<>
	static inline void LuaPushValToStack<const char *>(lua_State *L, const char * val)
	{
		//lua_pushstring(L, val);
	}

	template<>
	static inline void LuaPushValToStack<std::string>(lua_State *L, std::string val)
	{
		//lua_pushstring(L, val.c_str());
	}

	template<>
	static inline void LuaPushValToStack<StaticString<16>>(lua_State *L, StaticString<16> val)
	{
		//lua_pushstring(L, val.cstr());
	}

	template<typename T>
	static inline T getLuaValFromStack(lua_State *L, int index)
	{
		//static_assert(std::is_integral<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value, "Given type was not a number or pointer, can't guess at what it was.");
		//if constexpr(std::is_integral<T>::value)
		//{
		//	return static_cast<int>(luaL_checkinteger(L, index));
		//}
		//else if constexpr(std::is_pointer<T>::value)
		//{
		//	T *temp = (T*)lua_touserdata(L, index);
		//	return temp ? *temp : nullptr;
		//}
		//else if constexpr(std::is_enum<T>::value)
		//{
		//	return (T)luaL_checkinteger(L, index);
		//}
	}

	template <>
	static int getLuaValFromStack<int>(lua_State* L, int index)
	{
		//return (int)luaL_checkinteger(L, index);
	}

	template <>
	static unsigned int getLuaValFromStack<unsigned int>(lua_State* L, int index)
	{
		//return (unsigned int)luaL_checkinteger(L, index);
	}

	template <>
	static const char* getLuaValFromStack<const char*>(lua_State* L, int index)
	{
		//return luaL_checkstring(L, index);
	}

	template <>
	static StaticString<16> getLuaValFromStack<StaticString<16>>(lua_State* L, int index)
	{
		//return luaL_checkstring(L, index);
	}

	template <>
	static long getLuaValFromStack<long>(lua_State* L, int index)
	{
		//return (long)luaL_checkinteger(L, index);
	}

	template <typename T>
	static void BindEnum(const MetaEnum<T> &enum2Bind)
	{
		//Initialize();
		//std::string tableName = "luaL_";
		//tableName += enum2Bind._name;
		//luaL_newmetatable(_theState, tableName.c_str());
		//lua_pushvalue(_theState, -1);
		//lua_setfield(_theState, -1, "__index");
		//
		//for (auto val : enum2Bind._values)
		//{
		//	lua_pushinteger(_theState, val.second);
		//	lua_setfield(_theState, 1, val.first.c_str());
		//}
		//
		//lua_setglobal(_theState, enum2Bind._name);
	}

private:
	static const char *getTableName(size_t typeID);
	static void BindInternal(const std::pair<size_t, ClassToRegister> &thingToRegister);
	static void *luaAlloc(void *ud, void *ptr, size_t osize, size_t nsize);
	static std::unordered_map<size_t, ClassToRegister> &ThingsToRegister();
	static std::unordered_set<size_t> ThingsThatAreRegistered();
	//static std::unordered_map<size_t, std::unordered_map<const char *, lua_CFunction>> LuaIndexGettors;
	static lua_State *_theState;
};
