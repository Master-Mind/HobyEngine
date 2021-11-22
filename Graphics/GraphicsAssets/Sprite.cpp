#include "Sprite.h"
#include <boost/filesystem/path.hpp>
#include "glad/glad.h"
#include "Debugger.h"
#include "../Editor/FileDialog.h"

Sprite::Sprite()
{
}

void Sprite::Use(int texUnit) const
{
	_myTex->Use(texUnit);
}

void Sprite::Load(boost::filesystem::path filename)
{
	std::ifstream dir(filename.string() + ".sprt");

	//read in texture and shader paths
	char tex[MAX_PATH_LEN];
	dir.getline(tex, MAX_PATH_LEN);
	char shad[MAX_PATH_LEN];
	dir.getline(shad, MAX_PATH_LEN);
	_myTex = AssetDataBase::RequestAsset<Texture>(tex);
	_myShader = AssetDataBase::RequestAsset<Shader>(shad);
}

void Sprite::Create(std::string&& name, unsigned char* data, int width, int height)
{
	_myTex->Bind(data, width, height, false);
}

void Sprite::LoadShader(AssetName name)
{
	_myShader = AssetDataBase::RequestAsset<Shader>(name);
}
