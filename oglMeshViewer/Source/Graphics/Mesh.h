#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <iostream>

#include "../Util/Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Texture
{
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;
	std::vector<Vertex> m_vertices;

	Mesh(std::vector<GLuint> indices,
		std::vector<Texture> textures,
		std::vector<Vertex> vertices);

	void Draw(Shader& shader);

private:
	GLuint m_vao, m_vbo, m_ebo;

	void SetupMesh();
};
