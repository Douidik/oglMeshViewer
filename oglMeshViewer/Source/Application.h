#pragma once
#include <gl/glew.h>
#include "Window.h"
#include "Graphics/Model.h"
#include "Util/Matrix.h"

class Application
{
public:
	Application();
	void Update();

private:
	Model testModel;
	Shader m_shaderPhong, m_shaderNoLight;
	Shader* m_shader;
	Camera m_camera;
};