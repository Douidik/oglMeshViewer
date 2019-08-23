#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "../Camera.h"
#include "../Entity.h"

class Camera;

namespace Math
{
	glm::mat4 CreateViewMatrix(Camera* cam);
	glm::mat4 CreateModelMatrix(Entity& entity);
	glm::mat4 CreateProjMatrix();
}
