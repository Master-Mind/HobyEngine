#pragma once
#include "Asset.h"
#include "Graphics.h"

namespace boost {namespace filesystem {
	class path;
}
}

class Texture : public Asset
{
public:
	GRAPHICS_FRIENDS;

	explicit Texture()
		: _texID(-1)
	{
	}

	void Load(boost::filesystem::path &filename);
	void Use(int texUnit) const;
	void Bind(unsigned char* data, int width, int height, bool RGBA);
private:
	unsigned _texID;
};
