//#include "sbool.h"
#include "MetaDB.h"
//#include "MetaData.h"
//extern "C"
//{
//	#include <lualib.h>
//}
//#include <cstdlib>
//static const luaL_Reg debugFuncs[] = {
//{"DebugLog", LuaHandler::LuaDebugFuncWrapper<decltype(Debugger::Log)>},
////{ "cos",   LuaHandler::TheLuaFunc },
////{ "sin",   LuaHandler::TheLuaFunc },
//{nullptr, nullptr }
//};
//
//lua_State *LuaHandler::_theState;
//std::unordered_map<size_t, LuaHandler::ClassToRegister> &LuaHandler::ThingsToRegister()
//{
//	static std::unordered_map<size_t, ClassToRegister> map;
//	return map;
//}
//std::unordered_set<size_t> LuaHandler::ThingsThatAreRegistered()
//{
//	std::unordered_set<size_t>  map;
//	return map;
//}
//void LuaHandler::Initialize()
//{
//	static sbool _initialized;
//	if(!_initialized.Initialized())
//	{
//		_initialized.SetInitialized(true);
//		_theState = lua_newstate(luaAlloc, nullptr);
//		lua_atpanic(_theState, &LuaPanic);
//	}
//}
//
void LuaHandler::StartUp()
{
//	for(auto& it : ThingsToRegister())
//	{
//		BindInternal(it);
//	}
}
//

void LuaHandler::TestLua()
{
//	const char *buff = "foo = GameObjFactory.CreateObject(nil,\"time warner\")\n"
//	"Debugger.Log(foo:GetName(), DebugTag.Default)\n"
//	"foo:SetName(\"comcast\")"
//	"Debugger.Log(foo:GetName(), DebugTag.Default)\n"
//	"GameObjFactory.DestroyObject(foo)\n";
//	auto error = luaL_loadbuffer(_theState, buff, strlen(buff), "fuf") ||
//		lua_pcall(_theState, 0, 0, 0);
//	if (error) {
//		auto err = lua_tostring(_theState, -1);
//		DEBUGASSERTMSG(false, err, DTag::Lua);
//		lua_pop(_theState, 1);  /* pop error message from the stack */
//	}
}
//
//int LuaHandler::TheLuaFunc(lua_State* L)
//{
//	auto str = luaL_checkstring(L, 1);
//	DEBUGLOG(str, DTag::Lua);
//	return 0;
//}
//
//int LuaHandler::LuaPanic(lua_State* L)
//{
//	auto err = lua_tostring(L, -1);
//
//	DEBUGASSERTMSG(false, err, DTag::Lua);
//
//	return 0;
//}
//
//void LuaHandler::BindClass(ClassToRegister &&class_)
//{
//	ThingsToRegister().insert({ class_.ref->GetID(), class_ });
//}
//
//const char* LuaHandler::getTableName(size_t typeID)
//{
//	return MetaDB::getMetaName(typeID);
//}
//
//void LuaHandler::BindInternal(const std::pair<size_t, ClassToRegister>& thingToRegister)
//{
//	if(!ThingsThatAreRegistered().count(thingToRegister.first))
//	{
//		std::string metaName = "luaL_";
//		metaName.append(thingToRegister.second.ref->GetName());
//		luaL_newmetatable(_theState, metaName.c_str());
//
//		if (thingToRegister.second.staticReg.size())
//		{
//			luaL_setfuncs(_theState, thingToRegister.second.staticReg.data(), 0);
//			lua_pushvalue(_theState, -1);
//		}
//		
//
//		if (thingToRegister.second.dynamicReg.size())
//		{
//			luaL_setfuncs(_theState, thingToRegister.second.dynamicReg.data(), 0);
//			lua_pushvalue(_theState, -1);
//		}
//
//		//if(false && thingToRegister.second.ref->GetMemberCount()) //stuff for binding member variables directly, probs won't bother finishing this
//		//{
//		//	for (int i = 0, j = 0; i < thingToRegister.second.ref->GetMemberCount(); ++i)
//		//	{
//		//		if (thingToRegister.second.ref->GetMember(i)->HasProperty<MetaAttr::BindToLua>())
//		//		{
//		//			lua_pushstring(_theState, thingToRegister.second.ref->GetMember(i)->GetName());
//		//			lua_pushnumber(_theState, j);
//		//			// Stk: metatable, string, number
//		//			// Use lua_rawset() instead of lua_settable() to avoid
//		//			// the __newindex call
//		//			lua_rawset(_theState, -3);
//		//			j++;
//		//		}
//		//	}
//		//	luaL_Reg temp[] = { { "__index", thingToRegister.second.indexerForGetting },
//		//	{ "__newindex", thingToRegister.second.indexerForSetting },
//		//		{ nullptr, nullptr } };
//		//	luaL_setfuncs(_theState, temp, 0);
//		//	lua_pushvalue(_theState, -1);
//		//}
//
//		lua_setfield(_theState, -1, "__index");
//		lua_setglobal(_theState, thingToRegister.second.ref->GetName());
//		ThingsThatAreRegistered().insert(thingToRegister.first);
//	}
//}
//
//void* LuaHandler::luaAlloc(void* ud, void* ptr, size_t osize, size_t nsize)
//{
//	if(nsize == 0)
//	{
//		free(ptr);
//		return nullptr;
//	}
//
//	return realloc(ptr, nsize);
//}
