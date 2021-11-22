#include "VString.h"
#include "MetaDB.h"

DEFINE_META(VString)
END_META(VString)

VString::VString() : _str(nullptr), _len(0)
{
	
}

VString::VString(const char* str)
{
}

VString::VString(const VString& other)
{
}

VString& VString::operator=(const VString& other)
{
	return *this;
}

size_t VString::size() const
{
	return length();
}

size_t VString::length() const
{
	return _len;
}

void VString::resize(size_t new_size)
{
}

size_t VString::capacity() const
{
	return _capacity;
}

void VString::reserve(size_t new_capacity)
{
}

void VString::clear(size_t new_capacity)
{
}

void VString::empty(size_t new_capacity)
{
}

void VString::shrink_to_fit(size_t new_capacity)
{
}

char& VString::operator[](size_t index)
{
	return _str[index];
}

char VString::operator[](size_t index) const
{
	return _str[index];
}

char& VString::at(size_t index)
{
	return _str[index];
}

char VString::at(size_t index) const
{
	return _str[index];
}

VString& VString::operator+=(const VString& other)
{
	return *this;
}

VString VString::operator+(const char* str) const
{
	return VString();
}

void VString::append(const VString& other)
{
}

void VString::append(const char* str)
{
}

void VString::push_back(char c)
{
}

void VString::pop_back()
{
}

const char* VString::c_str() const
{
	return _str;
}

void VString::format_assign(const char* format, ...)
{
}
