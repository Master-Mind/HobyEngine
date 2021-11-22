#include "MetaAttribute.h"

AttribList &AttribList::operator=(const AttribList &rhs)
{
	_heapAlloced = true;
	_list.clear();
	_list.reserve(rhs._list.size());

	for (auto item : rhs._list)
	{
		_list.insert({ item.first, item.second->clone() });
	}

	return *this;
}

void AttribList::Clear()
{
	if (_heapAlloced)
	{
		for (auto item : _list)
		{
			delete item.second;
		}
	}
}

AttribList::~AttribList()
{
	Clear();
}
