#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include "../Util/Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Texture
{
	std::string type;
	std::string path;
	GLuint id;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	std::vector<Vertex>m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	void Draw(Shader& shader);

private:
	unsigned int m_vao, m_vbo, m_ebo;
};