#include "Shader.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Debugger.h"
#include "MetaDB.h"
#include "Graphics.h"

void Shader::Load(boost::filesystem::path vertPath, boost::filesystem::path fragPath)
{
	std::ifstream vertFile(vertPath.string());
	std::ifstream fragFile(fragPath.string());

	std::stringstream vertFileS, fragFileS;
	vertFileS << vertFile.rdbuf();
	fragFileS << fragFile.rdbuf();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	std::string aaahhh = vertFileS.str();
	std::string aaahhhh = fragFileS.str();
	const char *vertV = aaahhh.c_str();
	const char *fragV = aaahhhh.c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertV, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(vertex, 512, NULL, infoLog);
	DEBUGASSERTMSG(success, "Compilation failed for " + vertPath.string() + ": " + infoLog, DTag::AssetDatabase | DTag::Graphics);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragV, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(fragment, 512, NULL, infoLog);
	DEBUGASSERTMSG(success, "Compilation failed for " + fragPath.string() + ": " + infoLog, DTag::AssetDatabase | DTag::Graphics);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(ID, 512, NULL, infoLog);
	DEBUGASSERTMSG(success, std::string("Linking of shader program failed failed: ") + infoLog, DTag::AssetDatabase | DTag::Graphics);

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

unsigned Shader::GetID() const
{
	return ID;
}

void Shader::SetBool(const char* name, bool val) const
{
	glUniform1i(glGetUniformLocation(ID,name), val);
}

void Shader::SetFloat(const char* name, float val) const
{
	glUniform1f(glGetUniformLocation(ID, name), val);
}

void Shader::SetInt(const char* name, int val) const
{
	glUniform1i(glGetUniformLocation(ID, name), val);
}

void Shader::SetTex(const char* name, U32 val) const
{
	auto temp = glGetUniformLocation(ID, name);
	Graphics::ENGINE->SuppressGraphicsErrors();
	glUniform1ui(temp, val);
	Graphics::ENGINE->UnSuppressGraphicsErrors();
}

void Shader::SetMat4(const char* name, glm::mat4x4 val) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetVec2(const char* name, glm::vec2 val) const
{
	glUniform2f(glGetUniformLocation(ID, name), val.x, val.y);
}

void Shader::SetVec3(const char* name, glm::vec3 val) const
{
	glUniform3f(glGetUniformLocation(ID, name), val.x, val.y, val.z);
}

void Shader::SetVec4(const char* name, glm::vec4 val) const
{
	glUniform4f(glGetUniformLocation(ID, name), val.x, val.y, val.z, val.w);
}

void Shader::CheckError() const
{

	char logstr[512];
	int len = 0;
	glGetShaderInfoLog(ID, 512, &len, logstr);

	DEBUGASSERTMSG(len == 0, logstr, DTag::AssetDatabase | DTag::Graphics);
}


DEFINE_META(Shader)
_toStr = [](Shader &shad) {return shad.name.toString(); };
_fromStr = [](const char *str) { return *AssetDataBase::RequestAsset<Shader>(str); };
END_META(Shader)