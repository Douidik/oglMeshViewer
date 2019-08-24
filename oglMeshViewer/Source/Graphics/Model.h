#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "../Util/Shader.h"
#include "../Entity.h"
#include "Mesh.h"
#include "../../Vendor/stbi/stb_image.h"
#include <vector>

class Model : public Entity
{
public:
	Model(const std::string& path);
	void Draw(Shader& shader);

private:
	std::vector<Texture> m_textureCache;
	std::string m_directory;
	std::vector<Mesh> m_meshes;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	
	std::vector<Texture> LoadMaterialTexture(aiMaterial* mat, std::string typeName, aiTextureType type);

};