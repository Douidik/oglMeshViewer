#include "Model.h"

GLuint LoadTexture(const std::string& path, const std::string& directory)
{
	std::string filename = directory + '/' + path;
	GLuint id;
	glGenTextures(1, &id);

	int x, y, bpp;
	unsigned char* data = stbi_load(filename.c_str(), &x, &y, &bpp, 0);

	if (data)
	{
		GLenum format = ((bpp == 4) ? GL_RGBA : (bpp == 3) ? GL_RGB : (bpp == 1) ? GL_RED : GL_RGB);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Can't load texture: " << filename << std::endl;
		stbi_image_free(data);
	}

	return id;
}

Model::Model(const std::string& path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Draw(shader);
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	//vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 vector;
		Vertex vertex;

		//position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.position = vector;

		//normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.normal = vector;

		//uv
		if (mesh->mTextureCoords[0]) //the mesh contains uv ?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.uv = vec;
		}

		vertices.push_back(vertex);
	}	

	//indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//textures
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	//diffuse
	auto diffuseMaps = LoadMaterialTexture(mat, "texture_diffuse", aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//specular
	auto specularMaps = LoadMaterialTexture(mat, "texture_specular", aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//normal
	auto normalMaps = LoadMaterialTexture(mat, "texture_normal", aiTextureType_NORMALS);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	//height
	auto heightMaps = LoadMaterialTexture(mat, "texture_height", aiTextureType_HEIGHT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* mat, std::string typeName, aiTextureType type)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		bool skip = false;

		for (unsigned int j = 0; j < m_textureCache.size(); j++)
		{
			if (std::strcmp(path.C_Str(), m_textureCache[j].path.data()) == 0)
			{
				textures.push_back(m_textureCache[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = LoadTexture(path.C_Str(), m_directory);
			texture.path = path.C_Str();
			texture.type = typeName;
			textures.push_back(texture);
			m_textureCache.push_back(texture);
		}
	}

	return textures;
}
