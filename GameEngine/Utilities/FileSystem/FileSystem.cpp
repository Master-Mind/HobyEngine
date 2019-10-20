#include "FileSystem.h"
#include <fstream>
#include <boost/filesystem.hpp>
#include "Debugger.h"
#include "GameSerializer.h"
#include "GameObject.h"

GameObject* FileSystem::LoadLevel(const char* filename)
{
	boost::filesystem::path fpath = "Assets/levels/";
	std::string str(filename);

	fpath.append(str + ".xml");
	DEBUGASSERT(boost::filesystem::exists(fpath), DTag::FileSystem);
	str.clear();
	loadWholeFile(str, fpath);

	return GameSerializer::DeSerialize(const_cast<char*>(str.c_str()));
}

void FileSystem::loadWholeFile(std::string& outstr, boost::filesystem::path &filename)
{
	std::ifstream t(filename.c_str());
	outstr = std::string((std::istreambuf_iterator<char>(t)),
						  std::istreambuf_iterator<char>());
}
