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
#include <boost/interprocess/sync/named_mutex.hpp>
using namespace std::filesystem;
using namespace boost::interprocess;
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

bool moveToSolutionDir(const char *solutionName)
{
	while (!exists(solutionName))
	{
		current_path("../");
	}

	return exists(solutionName);
}

int main(int argc, char *argv[])
{
	named_mutex m(open_or_create, "PhilsSuperCoolReflectionParser");
	scoped_lock<named_mutex> sl(m, try_to_lock);

	if(!sl)
	{
		return 0;
	}
	
	if(!argv[1])
	{
		printf("Cannot run without a proper solution name\n");

		return 1;
	}
	
	if(argv[2] && strcmp(argv[3], "-w") == 0)
	{
		while(!IsDebuggerPresent()){}
	}

	try
	{
		if(!moveToSolutionDir(argv[1]))
		{
			printf("Could not find the solution %s in the running directory or any parent directories, aborting\n", argv[1]);
			
			return 2;
		}

		if(!FileTimeChecker::Init())
		{
			printf("An instance of codegen is already running, blocking build till it finishes...\n");
			fflush(stdout);
			
			while (!FileTimeChecker::Init())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			
			return 0;
		}
		
		printf("Running on %s\n", argv[1]);

		size_t arglen = strlen(argv[1]);
		if (argv[1][arglen - 1] == '\\')
			argv[1][arglen - 1] = 0;

		IncludesManager::Init();
		std::vector<path> projpaths;
		GetFiles(current_path(), projpaths);

		FileParser file;

		std::vector<path> sourcePaths;

		for (path& projPath : projpaths)
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
	catch(std::exception &exc)
	{
		printf("UNHANDLED EXCEPTION \"%s\", ABORTING\n", exc.what());
		
		return 3;
	}
}
