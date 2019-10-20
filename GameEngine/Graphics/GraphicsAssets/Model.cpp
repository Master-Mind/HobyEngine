#include "Model.h"
#include "glad/glad.h"

void Model::SetVerts(std::vector<Vertex>&& newVerts, std::vector<unsigned> &&newIndecies)
{
	_verts = newVerts;
	_indicies = newIndecies;
}

void Model::Bind()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, _verts.size() * sizeof(Vertex), &_verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicies.size() * sizeof(unsigned int),
		&_indicies[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

	glBindVertexArray(0);
}

void Model::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, _indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
