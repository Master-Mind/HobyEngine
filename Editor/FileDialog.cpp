#include "FileDialog.h"
#include <wchar.h>
#include <imgui.h>
#include "AssetDataBase.h"
#include "Scene.h"
#include "Scenemanager.h"
using namespace boost::filesystem;
FileDialog::FileDialog() : _active(false), curpath(), curPathStr{0}
{
}

bool FileDialog::Update()
{
	ImGui::Begin("File Dialogue", nullptr, ImGuiWindowFlags_MenuBar);
	ImGui::BeginMenuBar();
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.5,0,0,1 });
	ImGui::InputText("Path: ", curPathStr, MAX_PATH_LEN);
	if (ImGui::Button("X"))
	{
		_active = false;
	}
	ImGui::PopStyleColor();
	ImGui::EndMenuBar();
	if(_active)
	{
		for (auto& file : filesInDir)
		{
			if(ImGui::Button(file.c_str()))
			{
				path tempPath = curpath;
				tempPath.append(file);
				if(is_directory(tempPath))
				{
					UpdateDir(tempPath);
				}
				else if(is_regular_file(tempPath))
				{
					_active = false;
					curpath = tempPath;
				}
			}
		}

	}
	ImGui::End();
	return Active();
}

bool FileDialog::Active()
{
	return _active;
}

void FileDialog::Activate()
{
	path p = ("Assets/Levels");
	UpdateDir(p);
	_active = true;
}

void FileDialog::UpdateDir(path &newPath)
{
	curpath = newPath;
	std::strcpy(curPathStr, absolute(curpath).string().c_str());
	filesInDir.clear();
	for (auto dir = directory_iterator(curpath); dir != directory_iterator(); ++dir)
	{
		filesInDir.push_back(dir->path().filename().string());
	}
}
