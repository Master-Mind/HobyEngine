#pragma once

class VString
{
public:
	VString();
	VString(const char *str);
	VString(const VString &other);
	VString &operator=(const VString &other);
	size_t size() const;
	size_t length() const;
	void resize(size_t new_size);
	size_t capacity() const;
	void reserve(size_t new_capacity);
	void clear(size_t new_capacity);
	void empty(size_t new_capacity);
	void shrink_to_fit(size_t new_capacity);
	char &operator[](size_t index);
	char operator[](size_t index) const;
	char &at(size_t index);
	char at(size_t index) const;
	VString &operator+=(const VString &other);
	VString operator+(const char *str) const;
	void append(const VString &other);
	void append(const char *str);
	void push_back(char c);
	void pop_back();
	const char *c_str() const;
	static const size_t npos;
	void format_assign(const char *format, ...);
private:
	char *_str;
	size_t _len;
	size_t _capacity;
};