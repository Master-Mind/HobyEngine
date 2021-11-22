#include "IncludesManager.h"
#include "rapidjson/document.h"
#include <filesystem>

using namespace std::filesystem;
std::vector<path> IncludesManager::_includePaths;
const char includedirloc[] = "./includedirs.json";


void IncludesManager::Init()
{
	FILE* infile = fopen(includedirloc, "r");
	if(infile)
	{
		size_t insize = file_size(includedirloc);
		char* intext = new char[insize + 1];

		fread(intext, 1, insize, infile);
		intext[insize] = 0;

		rapidjson::Document doc;

		doc.ParseInsitu(intext);

		for (int i = 0; i < doc.Size(); ++i)
		{
			_includePaths.push_back(doc[i].GetString());
		}

		fclose(infile);
	}

}

const std::vector<std::filesystem::path>& IncludesManager::GetIncludePaths()
{
	return _includePaths;
}
