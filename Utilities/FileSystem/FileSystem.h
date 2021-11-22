#pragma once
#include <string>

namespace boost {namespace filesystem {
	class path;
}
}
#define MAX_PATH_LEN 512
class GameObject;
class FileSystem
{
public:
	static GameObject * LoadLevel(const char *filename);
private:
	static void loadWholeFile(std::string &outstr, boost::filesystem::path &filename);
};