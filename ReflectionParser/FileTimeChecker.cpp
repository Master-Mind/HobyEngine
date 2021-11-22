#include "FileTimeChecker.h"
#include <filesystem>
#include <cassert>
#include <rapidxml_print.hpp>
using namespace std::filesystem;

const char fileTimesLoc[] = "./ReflectionParser/dummyfilefortheparsertopoke.txt";

file_time_type FileTimeChecker::lastRunTime;

void FileTimeChecker::Init()
{
	if (!exists(fileTimesLoc))
	{
		return;
	}

	lastRunTime = last_write_time(fileTimesLoc);
}

bool FileTimeChecker::IsDirty(const path& pathToCheck)
{
	assert(exists(pathToCheck));

	return lastRunTime < last_write_time(pathToCheck);
}

void FileTimeChecker::DeInit()
{
	FILE *outFile = fopen(fileTimesLoc, "w");
	if (!outFile)
		perror("fopen");
	
	assert(outFile);

	fwrite("f", 1, 1, outFile);

	fclose(outFile);
}
