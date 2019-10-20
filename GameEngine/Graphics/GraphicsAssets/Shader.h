#pragma once
#include "AssetDataBase.h"
#include <boost/filesystem/path.hpp>
#include <glm/mat4x4.hpp>


class Shader : public Asset
{
public:
	friend class AssetDataBase;
	void Load(boost::filesystem::path vertPath, boost::filesystem::path fragPath);
	void Use() const;
	unsigned GetID() const;
	void SetBool(const char *name, bool val) const;
	void SetFloat(const char *name, float val) const;
	void SetInt(const char *name, int val) const;
	void SetTex(const char *name, U32 val) const;
	void SetMat4(const char *name, glm::mat4x4 val) const;
	void SetVec2(const char *name, glm::vec2 val) const;
	void SetVec3(const char *name, glm::vec3 val) const;
	void SetVec4(const char *name, glm::vec4 val) const;
	void CheckError() const;
private:
	unsigned ID;
};
