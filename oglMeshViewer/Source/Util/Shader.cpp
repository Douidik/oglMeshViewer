#include "Shader.h"

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
	:m_vertexFile(vertexFile), m_fragmentFile(fragmentFile)
{
	Source src = GetSource();
	m_id = CreateProgram(src);
}

Shader::~Shader()
{
	//glcall(glDeleteProgram(m_id));
}

void Shader::bind() const
{
	glcall(glUseProgram(m_id));
}

void Shader::unbind() const
{
	glcall(glUseProgram(0));
}

Source Shader::GetSource() const
{
	std::ifstream stream_vertex(m_vertexFile, std::ios::in),
				  stream_fragment(m_fragmentFile, std::ios::in);

	std::stringstream sstr_vertex, sstr_fragment;
	
	char currentPath[MAX_PATH];
	GetModuleFileName(NULL, currentPath, MAX_PATH);
	std::string str_currentPath = currentPath;
	std::replace(str_currentPath.begin(), str_currentPath.end(), '\\', '/');


	if (stream_vertex && stream_fragment)
	{
		sstr_vertex << stream_vertex.rdbuf();
		sstr_fragment << stream_fragment.rdbuf();
	}
	else
	{
		std::cerr << "Can't read " << ((!stream_vertex) ? "vertex" : "fragment") << " shader in file: "
			<< str_currentPath << ((!stream_vertex) ? m_vertexFile : m_fragmentFile) << ".\n";
	}

	return { sstr_vertex.str(), sstr_fragment.str() };
}

GLuint Shader::CreateProgram(Source& source)
{
	glcall(GLuint vertexID = glCreateShader(GL_VERTEX_SHADER));
	glcall(GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER));

	CompileShader(vertexID, source.vertex, GL_VERTEX_SHADER);
	CompileShader(fragmentID, source.fragment, GL_FRAGMENT_SHADER);

	glcall(GLuint prog = glCreateProgram());

	glcall(glAttachShader(prog, vertexID));
	glcall(glAttachShader(prog, fragmentID));

	glcall(glLinkProgram(prog));

	glcall(glDetachShader(prog, vertexID));
	glcall(glDetachShader(prog, fragmentID));

	glcall(glDeleteShader(vertexID));
	glcall(glDeleteShader(fragmentID));

	return prog;
}

void Shader::CompileShader(GLuint& id, const std::string& src, GLenum type)
{
	const char* cc_src = src.c_str();
	glcall(glShaderSource(id, 1, &cc_src, nullptr));
	glcall(glCompileShader(id));

	int res, len; 
	
	glcall(glGetShaderiv(id, GL_COMPILE_STATUS, &res));
	glcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));

	if (!res)
	{
		std::vector<char> log(len + 1);
		glcall(glGetShaderInfoLog(id, len, nullptr, &log[0]));
		
		std::cerr << ((type == GL_VERTEX_SHADER) ? "Vertex shader error: " : "Fragment shader error: ")
				  << std::endl;

		for (int i = 0; i < len; i++)
			std::cerr << log[i];
	}
}

//UNIFORMS

int Shader::GetLocation(const std::string& name)
{
	if (m_uniformLocation.find(name) != m_uniformLocation.end())
		return m_uniformLocation[name];

	glcall(int location = glGetUniformLocation(m_id, name.c_str()));

	if (location == -1)
	{
		std::cerr << "Can't find uniform " << name << "." << std::endl;
	}

	m_uniformLocation[name] = location;

	return location;
}

void Shader::Set1f(const std::string& name, float value)
{
	bind();
	glcall(glUniform1f(GetLocation(name), value));
}

void Shader::Set3f(const std::string& name, glm::vec3& value)
{
	bind();
	glcall(glUniform3f(GetLocation(name), value.x, value.y, value.z));
}

void Shader::Set4f(const std::string& name, glm::vec4& value)
{
	bind();
	glcall(glUniform4f(GetLocation(name), value.x, value.y, value.z, value.w));
}

void Shader::Set1i(const std::string& name, int value)
{
	bind();
	glcall(glUniform1i(GetLocation(name), value));
}

void Shader::SetMat4(const std::string& name, glm::mat4& value)
{
	bind();
	glcall(glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::SetMat3(const std::string& name, glm::mat3& value)
{
	bind();
	glcall(glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::SetModelMatrix(glm::mat4& model)
{
	SetMat4("model", model);
}

void Shader::SetViewMatrix(glm::mat4& view)
{
	SetMat4("view", view);
}

void Shader::SetProjMatrix(glm::mat4& proj)
{
	SetMat4("proj", proj);
}
