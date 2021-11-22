#include <filesystem>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <mutex>
#include "FileParser.h"
#include "Windows.h"
#include "FileTimeChecker.h"
#include "IncludesManager.h"
#include "VSProjParser.h"

using namespace std::filesystem;
void GetFiles(path cur, std::vector<path> &out)
{
	if(is_directory(cur))
	{
		directory_iterator dir(cur);

		while(dir != directory_iterator())
		{
			GetFiles(*dir++, out);
		}
	}
	else
	{
		path ext = cur.extension();

		if (ext == ".vcxproj")
		{
			out.push_back(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	assert(argv[1]);
	if(argv[2] && strcmp(argv[2], "-w") == 0)
	{
		while(!IsDebuggerPresent()){}
	}

	printf("Running on %s\n", argv[1]);

	current_path(argv[1]);

	size_t arglen = strlen(argv[1]);
	if(argv[1][arglen - 1] == '\\')
		argv[1][arglen - 1] = 0;

	IncludesManager::Init();
	FileTimeChecker::Init();
	std::vector<path> projpaths;
	GetFiles(current_path(), projpaths);

	FileParser file;

	std::vector<path> sourcePaths;

	for (path &projPath : projpaths)
	{
		if (projPath.string().find("ReflectionParser") == std::string::npos)
		{
			file.AddProject(new VSProjParser(projPath));
		}
	}

	file.Parse();


	FileTimeChecker::DeInit();

	return 0;
}
