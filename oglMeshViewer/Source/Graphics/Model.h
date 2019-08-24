#pragma once 
#include <gl/glew.h>
#include <glm.hpp>
#include "../../Vendor/stbi/stb_image.h"
#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Entity.h"
#include "../Util/Shader.h"
#include "Mesh.h"

class Model : public Entity
{
public:
	Model(const std::string& path);
	Model() = default;
	void Draw(Shader& shader);

	std::vector<Texture> m_textureLoaded;
	std::vector<Mesh> m_meshes;

	//this string will contain the path of the folder containing the files
	std::string m_directory;

private:
	void LoadModel(const std::string& path);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);
	
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name);

};