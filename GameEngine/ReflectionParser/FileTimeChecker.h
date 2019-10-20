#pragma once
#include <filesystem>
#include <unordered_map>
#include <string>

class FileTimeChecker
{
public:
	//reads in file time file and records the times
	static void Init();
	//returns whether the given file has been edited since reflection parse has last seen it
	static bool IsDirty(const std::filesystem::path &pathToCheck);
	//saves current filetimes
	static void DeInit();
private:
	static std::filesystem::file_time_type lastRunTime;
};
