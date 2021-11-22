#pragma once
//#include "Debugger.h"
#include <string>

template<int size>
class StaticString
{
public:
	StaticString() : _len(-1)
	{
		
	}
	StaticString(const StaticString<size> &rhs)
	{
		*this = rhs;
	}
	StaticString(const char *str)
	{
		*this = str;
	}
	StaticString(const std::string &str)
	{
		*this = str;
	}

	operator const char *()
	{
		return &_str[0];
	}
	std::string toString()
	{
		std::string ret;
		ret.reserve(size);
		ret = _str;
		return ret;
	}

	const char *cstr()
	{
		return &_str[0];
	}
	StaticString<size> &operator=(const StaticString<size> &rhs)
	{
		std::memcpy(reinterpret_cast<char *>(&_str[0]), &rhs._str[0], size);

		_len = rhs._len;

		return *this;
	}

	StaticString<size> &operator=(const std::string &rhs)
	{
		std::memcpy(reinterpret_cast<char *>(&_str[0]), rhs.c_str(), size);

		_len = rhs.size();

		return *this;
	}
	StaticString<size> &operator=(const char *rhs)
	{
		int i = 0;
		for (; rhs[i] && i < size - 1; ++i)
		{
			_str[i] = rhs[i];
		}

		_str[i] = rhs[i];//copy null
		_len = i;

		return *this;
	}

	bool operator==(const StaticString<size> &rhs)const
	{
		return lengthsAreSame(rhs) && strcmp(_str, rhs._str) == 0;
	}
	bool operator!=(const StaticString<size> &rhs)const
	{
		return !(*this == rhs);
	}
	bool operator<(const StaticString<size> &rhs)const
	{
		return strcmp(_str, rhs._str) < 0;
	}
	bool operator>(const StaticString<size> &rhs)const
	{
		return strcmp(_str, rhs._str) > 0;
	}
	bool operator<=(const StaticString<size> &rhs)const
	{
		return strcmp(_str, rhs._str) <= 0;
	}
	bool operator>=(const StaticString<size> &rhs)const
	{
		return strcmp(_str, rhs._str) >= 0;
	}

	int GetLength()
	{
		if(_len != -1)
		{
			return _len;
		}

		_len = (int)strlen(_str);

		return _len;
	}

	friend struct DebugEvent;
private:

	bool lengthsAreSame(const StaticString<size> &rhs) const//returns true if either is -1
	{
		return (_len == -1 && rhs._len == -1) || _len == rhs._len;
	}
	char _str[size];
	int _len;
};
