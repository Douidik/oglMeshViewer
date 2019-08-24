#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	:m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);

	//normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

	//uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name = m_textures[i].type;
		unsigned int number = 0;

		if (m_textures[i].type == "texture_diffuse")
		{
			number = (diffuseNr++);
		}
		else if (m_textures[i].type == "texture_specular")
		{
			number = (specularNr++);
		}
		else if (m_textures[i].type == "texture_normal")
		{
			number = (normalNr++);
		}
		else if (m_textures[i].type == "texture_height")
		{
			number = (heightNr++);
		}

		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		shader.Set1i((name + std::to_string(number)), i);
	}

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
