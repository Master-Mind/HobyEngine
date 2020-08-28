#include "FileTimeChecker.h"
#include <filesystem>
#include <cassert>
#include <rapidxml_print.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
using namespace std::filesystem;
using namespace boost::interprocess;

const char fileTimesLoc[] = "./ReflectionParser/dummyfilefortheparsertopoke.txt";

file_time_type FileTimeChecker::lastRunTime;
FILE *FileTimeChecker::filetocheck = nullptr;

bool FileTimeChecker::Init()
{
	if (exists(fileTimesLoc))
	{
		lastRunTime = last_write_time(fileTimesLoc);
	}
	
	filetocheck = fopen(fileTimesLoc, "w");
	fwrite("f", 1, 1, filetocheck);

	fclose(filetocheck);
	if (!filetocheck)
		return false;

	return true;
}

bool FileTimeChecker::IsDirty(const path& pathToCheck)
{
	assert(exists(pathToCheck));

	return lastRunTime < last_write_time(pathToCheck);
}

void FileTimeChecker::DeInit()
{
}
