#pragma once
#include <boost/filesystem.hpp>
#include "FileSystem.h"
#include "StaticString.h"
#include "Asset.h"

class FileDialog
{
public:
	enum ActiveState
	{
		NotActive,

	};
	FileDialog();
	bool Update();
	bool Active();
	void Activate();
	template<typename T>
	AssetReference<T> loadSelectedFile()
	{
		auto str = curpath.stem().string();
		//str.pop_back();
		return AssetDataBase::RequestAsset<T>(str);
	}
private:
	void UpdateDir(boost::filesystem::path &newPath);
	bool _active;
	boost::filesystem::path curpath;
	char curPathStr[MAX_PATH_LEN];
	std::vector<std::string> filesInDir;
};
