#pragma once
#include <vector>
#include <filesystem>

class IncludesManager
{
public:
	static void Init();
	static const std::vector<std::filesystem::path>& GetIncludePaths();
private:
	static std::vector<std::filesystem::path> _includePaths;
};
