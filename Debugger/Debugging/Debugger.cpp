#include "Debugger.h"
#include <iostream>
#include <imgui.h>
#include <stdlib.h>
#include "MetaDB.h"
#include <boost/filesystem/path.hpp>
#include "../Graphics/Graphics.h"
#include <glm/gtc/matrix_transform.hpp>
METAENUM(MetaAttr::BindToLua(),DebugTag, U64,
	DTag::Default, 
	DTag::Assert,
	DTag::Warning,
	DTag::Graphics,
	DTag::Graphics, 
	DTag::Editor, 
	DTag::GameStructure,
	DTag::Memory, 
	DTag::Physics, 
	DTag::AI, 
	DTag::AssetDatabase, 
	DTag::WindowManagement, 
	DTag::Input,
	DTag::FileSystem,
	DTag::All);

const int Debugger::_maxLogNum = 2048;
DebugEvent Debugger::_log[DTag::Count * _maxLogNum];
int Debugger::_logStart = 0;
int Debugger::_logEnd = 0;
bool Debugger::_fillDaFukUP = false;
std::vector<DebugLineVert> Debugger::_lines;

DebugEvent::DebugEvent(std::pair<const char *, unsigned> filename, unsigned line, long flags, DebugStr& text_) : flags(flags)
{
	//copy file name
	std::memcpy(&text._str, filename.first, filename.second);
	//text=filename.ext
	int numLineDigs = line < 10 ? 1 : line < 100 ? 2 : line < 1000 ? 3 : 4;
	text._str[filename.second - 1] = ' ';
	//text=filename.ext 
	sprintf((&text._str[0]) + filename.second, "%u", line);
	//text=filename.ext l#
	text._str[filename.second + numLineDigs] = ':';
	text._len = filename.second + numLineDigs + 1;
	int minlen = std::min(text_.GetLength(), DEBUGSTRLEN - text._len);
	//text=filename.ext l#:
	memcpy(text._str + text._len, text_._str, minlen);
	text._len += minlen;
	text._str[text._len] = 0;
	//text=filename.ext l#:text
}

DebugEvent::DebugEvent()
{
}

std::pair<const char *, unsigned> getFileName(const char *file)
{
	const char *ret = file;
	int i = 0;
	for (; ; ++i)
	{
		if (file[i] == '\\')
		{
			ret = file + i;
		}
		else if(file[i] == 0)
		{
			if(ret == file)
			{
				return { ret, unsigned(file + i + 1 - ret) };
			}
			break;
		}
	}

	return { ret + 1, unsigned(file + i - ret)};
}

void Debugger::Log(const char *message, const char* file, unsigned line, DTag::DebugTag flags)
{
	if constexpr(IsInDebug)
	{
		PushToLog(std::move(DebugEvent(getFileName(file), line, flags, DebugStr(message))));
	}
}

bool Debugger::AssertFail(const char *condStr, const char* file, unsigned line, DTag::DebugTag flags)
{
	if constexpr(IsInDebug)
	{
		DebugStr str("Assertion: the given value was not true");
		PushToLog(std::move(DebugEvent(getFileName(file), line, flags, str)));
	}

	return false;
}

bool Debugger::AssertFail(DebugStr &&message, const char* file, unsigned line, DTag::DebugTag flags)
{
	if constexpr(IsInDebug)
	{
		boost::filesystem::path p(file);
		PushToLog(std::move(DebugEvent(getFileName(file), line, flags, message)));
	}

	return false;
}

void Debugger::DumpLogToEditWIndow(long flags)
{
	if(constexpr(IsInDebug))
	{
		//ImGui::Begin("Debug output");
		if(_fillDaFukUP)
		{
			PrintLog(_logStart, _maxLogNum, 0, _logEnd, flags);
		}
		else
		{
			PrintLog(0, _logEnd, 0, 0, flags);
		}
		//ImGui::End();
	}
}

void Debugger::DrawLine(glm::vec3 start, glm::vec3 end, Color color)
{
	_lines.push_back({start,color});
	_lines.push_back({ end,color });
}

void Debugger::DrawArrow(glm::vec3 start, glm::vec3 end, Color color)
{
	glm::vec3 toEnd = end - start;
	glm::vec3 relLeft = cross(toEnd, {1,1,1});
	glm::vec3 relup = cross(toEnd, relLeft);
	
	//the line
	DrawLine(start, end, color);
	//the base of the head
	glm::vec3 basepos = start + toEnd * 0.9f;
	DrawLine(basepos + relup * 0.1f, basepos - relup * 0.1f, color);
	DrawLine(basepos + relLeft * 0.1f, basepos - relLeft * 0.1f, color);
	//the diagonals
	DrawLine(basepos + relup * 0.1f, end, color);
	DrawLine(basepos - relup * 0.1f, end, color);
	DrawLine(basepos + relLeft * 0.1f, end, color);
	DrawLine(basepos - relLeft * 0.1f, end, color);
}

void Debugger::DrawCircle(glm::vec3 center, Color color)
{
}

void Debugger::DrawAll()
{
	Graphics::ENGINE->RenderDebug(_lines);
	_lines.clear();
}

void Debugger::PushToLog(DebugEvent&& message)
{
	if (_logEnd == _logStart && _logEnd == 0)
	{
		_log[_logEnd] = std::move(message);
		_logEnd++;
	}
	else if(_logEnd >= _maxLogNum)
	{
		if(_logStart == _maxLogNum)
		{
			_logStart = 0;
		}
		_fillDaFukUP = true;
		_logEnd = 0;
		_logStart++;
		_log[_logEnd] = std::move(message);
		_logEnd++;
	}
	else if(_logEnd == _logStart)
	{
		_logStart++;
		_log[_logEnd] = std::move(message);
		_logEnd++;
	}
	else
	{
		_log[_logEnd] = std::move(message);
		_logEnd++;
	}
}

void Debugger::PrintLog(int startStart, int maxLength, int startEnd, int endEnd, long flags)
{
	for (int i = startStart; i < maxLength; ++i)
	{
		if(_log[i].flags & flags)
		{
			ImGui::Text(_log[i].text);
		}
	}
	for (int i = startEnd; i < endEnd; ++i)
	{
		if (_log[i].flags & flags)
		{
			ImGui::Text(_log[i].text);
		}
	}
}
