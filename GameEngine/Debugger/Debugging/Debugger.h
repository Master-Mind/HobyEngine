#pragma once
#include <string>
#include "StaticString.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include "CoolTypeDefs.h"
#include "../Graphics/Color.h"
#define DEBUGSTRLEN 512
typedef StaticString<DEBUGSTRLEN> DebugStr;
namespace DTag
{
	const unsigned start = __COUNTER__;
#define GETVAL (__COUNTER__ - start - 1)
	enum DebugTag : U64
	{
		None = 0,
		Default = 1 << GETVAL,
		Assert = 1 << GETVAL,
		Warning = 1 << GETVAL,
		Verbose = 1 << GETVAL,
		Core = 1 << GETVAL,
		Graphics = 1 << GETVAL,
		Editor = 1 << GETVAL,
		GameStructure = 1 << GETVAL,
		Memory = 1 << GETVAL,
		Physics = 1 << GETVAL,
		AI = 1 << GETVAL,
		AssetDatabase = 1 << GETVAL,
		WindowManagement = 1 << GETVAL,
		Input = 1 << GETVAL,
		FileSystem = 1 << GETVAL,
		Lua = 1 << GETVAL,
		Testing = 1 << GETVAL,
		All = U64(~0),
	};
	const unsigned Count = GETVAL;
}

struct DebugEvent
{
	DebugStr text;
	unsigned line;
	long flags;
	DebugEvent(std::pair<const char *, unsigned>filename, unsigned line, long flags, DebugStr &text_);
	DebugEvent();
	//std::string toString();
};
struct DebugLineVert
{
	glm::vec3 pos;
	glm::vec4 color;
};
class [[MetaAttr::Reflect(), MetaAttr::BindToLua()]] Debugger
{
public:
	//template<typename T>
	//static void Log(T message, std::string file, unsigned line, DTag::DebugTag flags);
	static void Log(const char *message, const char* file, unsigned line, DTag::DebugTag flags);
	static bool AssertFail(const char *condStr, const char* file, unsigned line, DTag::DebugTag flags);
	static bool AssertFail(DebugStr &&message, const char* file, unsigned line, DTag::DebugTag flags);
	static void DumpLogToEditWIndow(long flags); 
	static void DrawLine(glm::vec3 start, glm::vec3 end, Color color = DColor::red);
	static void DrawArrow(glm::vec3 start, glm::vec3 end, Color color = DColor::red);
	static void DrawCircle(glm::vec3 center, Color color = DColor::red);
	static void DrawAll();
#ifdef DEBUG
	static const bool IsInDebug = true;
#else
	static const bool IsInDebug = false;
#endif
private:
	static inline void PushToLog(DebugEvent &&message);
	static void PrintLog(int startStart, int maxLength, int startEnd, int endEnd, long flags);
	static const int _maxLogNum;
	static DebugEvent _log[];
	static int _logStart;
	static int _logEnd;
	static bool _fillDaFukUP;
	static std::vector<DebugLineVert> _lines;
};

//template <typename T>
//void Debugger::Log(T message, std::string file, unsigned line, DTag::DebugTag flags)
//{
//	if constexpr(IsInDebug)
//	{
//		//Log(std::to_string(message), file, line, flags);
//	}
//}

#define DEBUGLOG(LOGSTR, TAG) Debugger::Log(LOGSTR, __FILE__, __LINE__, DTag::DebugTag(TAG))
#define DEBUGASSERTFAILED(STR, TAG) Debugger::AssertFail(STR, __FILE__, __LINE__, DTag::DebugTag(TAG | DTag::Assert))
#define DEBUGASSERT(COND, TAG) COND || DEBUGASSERTFAILED(#COND, TAG)
#define DEBUGASSERTMSG(COND, LOGSTR, TAG) COND ||  DEBUGASSERTFAILED(LOGSTR, TAG)