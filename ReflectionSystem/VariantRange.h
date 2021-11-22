#pragma once
#include <vector>
#include "MetaDB.h"

class VariantRange
{
public:
	template<typename T>
	static void AssignData(VariantRange &var, T &data)
	{
		for (auto elem : data)
		{
			var.GenericData.push_back(&elem);
		}

		var._dataMeta = GetMetaData<T>();
	}
	std::vector<void *> GenericData;
private:
	MetaReference _dataMeta;
};
