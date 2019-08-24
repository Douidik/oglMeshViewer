#pragma once
#include <gl/glew.h>
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw_gl3.h"
#include "Window.h"
#include "Graphics/Model.h"
#include "Util/Matrix.h"

class Application
{
public:
	Application();
	void Update();

private:
	Model m_model;
	Shader m_shaderPhong, m_shaderNoLight;
	Shader* m_shader;
	Camera m_camera;
};