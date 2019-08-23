#pragma once
#include <gl/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <windows.h>
#include <filesystem>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "../../Debug.h"

struct Source
{
	std::string vertex, fragment;
};

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vertexFile, const std::string& fragmentFile);
	~Shader();

	void bind() const;
	void unbind() const;

	inline GLuint GetID() const { return m_id; }
private:
	Source GetSource() const;
	GLuint CreateProgram(Source& source);
	void CompileShader(GLuint& id, const std::string& src, GLenum type);

private:
	GLuint m_id;
	std::string m_vertexFile, m_fragmentFile;

	//Uniforms
private:
	std::unordered_map < std::string, int > m_uniformLocation;
	int GetLocation(const std::string& name);

public:
	void Set1f(const std::string& name, float value);
	void Set3f(const std::string& name, glm::vec3& value);
	void Set4f(const std::string& name, glm::vec4& value);

	void Set1i(const std::string& name, int value);

	void SetMat4(const std::string& name, glm::mat4& value);
	void SetMat3(const std::string& name, glm::mat3& value);

	void SetModelMatrix(glm::mat4& model);
	void SetViewMatrix(glm::mat4& view);
	void SetProjMatrix(glm::mat4& proj);
};