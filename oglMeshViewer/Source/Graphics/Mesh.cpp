#include "Mesh.h"

Mesh::Mesh(std::vector<GLuint> indices, std::vector<Texture> textures, std::vector<Vertex> vertices)
	:m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	int diffuseNr = 0;
	int specularNr = 0;
	int normalNr = 0;
	int heightNr = 0;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string name = m_textures[i].type;
		std::string number;

		if (m_textures[i].type == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (m_textures[i].type == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (m_textures[i].type == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		else if (m_textures[i].type == "texture_height")
		{
			number = std::to_string(heightNr++);
		}

		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		shader.Set1i((name + number), i);
	}

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

//linking all buffers with vao

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

