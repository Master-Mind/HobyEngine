#include "VariableAllocator.h"
#include <vector>
#include <cassert>
#include <string>

VariableAllocator::VariableAllocator(unsigned allignment)
	: _allignment(allignment), _pageList(nullptr)
{
}

void* VariableAllocator::allocate(size_t size, const char *file, const char *function, U32 line)
{
	MemblockHeader *block = (MemblockHeader*)malloc(sizeof(MemblockHeader*) + size);
	block->FileName = file;
	block->FunctionName = function;
	block->LineNumber = line;
	tempMap[(size_t)block] = (char *)block + sizeof(block);

	return (char *)block + sizeof(block);
}

void VariableAllocator::deallocate(void* mem)
{
	assert(tempMap.count((size_t)mem));
	tempMap.erase((size_t)mem);
	free((char *)mem - sizeof(MemblockHeader));
}

std::vector<std::string> VariableAllocator::checkLeaks()
{
	std::vector<std::string> ret(tempMap.size());

	for (auto allocation : tempMap)
	{
		MemblockHeader *blockh = static_cast<MemblockHeader*>((void*)((char *)allocation.second - sizeof(MemblockHeader)));
		ret.push_back("Leak found from " + std::string(blockh->FileName) + blockh->FunctionName + std::to_string(blockh->LineNumber));
	}

	return ret;
}
