#pragma once
#include <vector>
#include "ObjectAllocator.h"
#include "Debugger.h"

//TODO: make an actual allocator

template<typename T>
class TypeAllocator
{
public:
	static void Init(size_t size)
	{
	}

	static T *Allocate()
	{
		T *ret = reinterpret_cast<T*>(Allocator.Allocate("wew"));
		new (ret) T;
		return ret;
	}

	static void DeAllocate(T *obj)
	{
		Allocator.Free(obj);
	}
private:
	static ObjectAllocator Allocator;
};

template <typename T>
ObjectAllocator TypeAllocator<T>::Allocator(sizeof(T), OAConfig(true, 128, DEFAULT_MAX_PAGES, Debugger::IsInDebug, 0, OAConfig::HeaderBlockInfo(), 8));