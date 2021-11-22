#pragma once
#include <vector>
#include <filesystem>

class VSProjParser
{
public:
	VSProjParser(std::filesystem::path &projFile = std::filesystem::current_path());
	const std::vector<std::filesystem::path>& GetHeaderFiles();
	const std::vector<std::string>& GetDependencies() const;
	const std::filesystem::path& GetPath();
	bool IsApp();
private:
	std::filesystem::path _projFile;
	std::vector<std::filesystem::path> _sourceFiles;
	std::vector<std::filesystem::path> _headerFiles;
	std::vector<std::string> _dependencies;
	//true if it compiles to exe, false otherwise
	bool _isApp = false;
};

