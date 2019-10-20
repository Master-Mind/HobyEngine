#pragma once
#include "Shader.h"
#include <imgui.h>
#include "AssetDataBase.h"
#include "Texture.h"
namespace boost {namespace filesystem {
	class path;
}
}

class Sprite : public Asset
{
public:
	GRAPHICS_FRIENDS;
	Sprite();
	void Use(int texUnit) const;
	void Load(boost::filesystem::path filename);
	void Create(std::string &&name, unsigned char *data, int width, int height);
	void LoadShader(AssetName name);
private:
	AssetReference<Shader> _myShader;
	AssetReference<Texture> _myTex;
};
