#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Util/Matrix.h"
#include "Window.h"
#include "Entity.h"

class Camera : public Entity
{
public:
	void input(float dt);

	inline const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
	inline const glm::mat4& getProjMatrix() const { return m_projMatrix; }

private:
	void mouseInput();
	double lastMousePosX, lastMousePosY;

	glm::mat4 m_viewMatrix, m_projMatrix;
};

