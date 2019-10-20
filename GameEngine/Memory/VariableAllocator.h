#pragma once
#include "MemCommon.h"
#include <unordered_map>

typedef unsigned short VPAddr;

class VariableAllocator
{
public:
	VariableAllocator(unsigned allignment);

	void *allocate(size_t size, const char *file, const char *function, U32 line);

	template<typename T>
	T *allocate(const char *file, const char *function, U32 line)
	{
		T *ret = static_cast<T*>(allocate(sizeof(T), file, function, line));
		new (ret) T();

		return ret;
	}

	template<typename T>
	T *allocate(T &val, const char *file, const char *function, U32 line)
	{
		T *ret = static_cast<T*>(allocate(sizeof(T), file, function, line));

		new (ret) T(val);

		return ret;
	}

	void deallocate(void *mem);
	std::vector<std::string> checkLeaks();

private:
	struct VPage
	{
		VPAddr free;
		VPAddr blockList;
		VPage *nextPage;
	};

	struct VBlock
	{
		bool inUse;
		VPAddr size;
		VPAddr next;
	};

	VPage *_pageList;
	unsigned _allignment;
	unsigned _leftallign;
	unsigned _rightallign;
	std::unordered_map<size_t, void *> tempMap;

};

#define VALLOC(ALLOCATOR, SIZE) \
	(ALLOCATOR)->allocate(SIZE, __FILE__, __FUNCTION__, __LINE__);

#define VTALLOC(ALLOCATOR, TYPE) \
	(ALLOCATOR)->allocate<TYPE>(__FILE__, __FUNCTION__, __LINE__);

#define VT_VAL_ALLOC(ALLOCATOR, TYPE, VAL) \
	(ALLOCATOR)->allocate<TYPE>(VAL, __FILE__, __FUNCTION__, __LINE__);