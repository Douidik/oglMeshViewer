#include "Model.h"

unsigned int TextureFromFile(const std::string& path, const std::string& directory);

Model::Model(const std::string& path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
		m_meshes[i].Draw(shader);
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
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
	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

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
		if (mesh->mTextureCoords[0]) //Does the mesh contains uv ?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.uv = vec;
		}
		else
			vertex.uv = glm::vec2(0.0f, 0.0f);

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
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//diffuse
	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//Specular
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	//Normal
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	//Height
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return(Mesh(indices, textures, vertices));
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name)
{
	std::vector<Texture> textures;
	//for each texture of type 
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString path;
		mat->GetTexture(type, i, &path);

		bool skip = false;

		//for each texture in loaded textures
		for (unsigned int j = 0; j < m_textureLoaded.size(); j++)
		{
			if (std::strcmp(path.C_Str(), m_textureLoaded[j].path.data()) == 0)
			{
				textures.push_back(m_textureLoaded[j]);
				skip = true;
				break;
				//texture is finded in cache
			}
		}
		//texture isn't in cache
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(path.C_Str(), m_directory);
			texture.path = path.C_Str();
			texture.type = name;
			textures.push_back(texture);		//in scope of this mesh
			m_textureLoaded.push_back(texture); //in scope this model
		}
	}

	return textures;
}

unsigned int TextureFromFile(const std::string& path, const std::string& directory)
{
	std::string filename = path;
	filename = directory + '/' + filename;

	int x, y, bpp;
	unsigned char* data = stbi_load(filename.c_str(), &x, &y, &bpp, 0);
	
	GLuint id;
	glGenTextures(1, &id);

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
