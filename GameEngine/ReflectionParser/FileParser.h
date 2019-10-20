#pragma once
#include <filesystem>
#include <mutex>
#include <unordered_map>
#include "VSProjParser.h"

typedef void *CXIndex;
class ASTWriter;

class FileParser
{
public:
	FileParser();
	~FileParser();
	void Parse();
	void AddProject(VSProjParser* proj);
private:
	void parseTreaded(std::vector<const std::filesystem::path*> filenames);
	std::unordered_map<std::string, VSProjParser*> _appProjects;
	std::unordered_map<std::string, VSProjParser*> _libProjects;
	std::vector<char *> cmdargs;
	std::mutex _writtersLock;
	std::unordered_map<std::string, ASTWriter* > _finalwriters;
	bool Translate(CXIndex index, const std::filesystem::path& filename);
};

