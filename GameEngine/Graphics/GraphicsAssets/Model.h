#pragma once
#include "AssetDataBase.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

class Model : public Asset
{
public:
	class Vertex
	{
	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 UV;
	};

	void SetVerts(std::vector<Vertex> &&newVerts, std::vector<unsigned> &&newIndecies);
	void Bind();
	void Draw();
private:
	std::vector<Vertex> _verts;
	std::vector<unsigned> _indicies;
	unsigned VAO, VBO, EBO;
};
