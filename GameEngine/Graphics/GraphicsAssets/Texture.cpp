#include "Texture.h"
#include <boost/filesystem.hpp>
#include "stb_image.h"
#include "glad/glad.h"
#include "Debugger.h"

void Texture::Load(boost::filesystem::path& filename)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename.string().c_str(), &width, &height, &nrChannels, 0);
	DEBUGASSERTMSG(data, "ERROR: texture" + filename.string() + "could not be loaded\n", DTag::AssetDatabase);
	Bind(data, width, height, filename.extension().c_str()[1] == L'p');
	stbi_image_free(data);

}

void Texture::Use(int texUnit) const
{
	if (_texID == -1)
	{
		return;
	}
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, _texID);
}

void Texture::Bind(unsigned char* data, int width, int height, bool RGBA)
{
	glGenTextures(1, &_texID);
	glBindTexture(GL_TEXTURE_2D, _texID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
}
