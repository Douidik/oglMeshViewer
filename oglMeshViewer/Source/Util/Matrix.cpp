#include "Matrix.h"

glm::mat4 Math::CreateViewMatrix(Camera* cam)
{
	glm::mat4 matrix(1.0f);

	//std::cout << cam->position.x << " " << cam->position.y << " " << cam->position.z << std::endl;

	matrix = glm::rotate(matrix, glm::radians(cam->rotation.x), { 1,0,0 });
	matrix = glm::rotate(matrix, glm::radians(cam->rotation.y), { 0,1,0 });
	matrix = glm::rotate(matrix, glm::radians(cam->rotation.z), { 0,0,1 });

	matrix = glm::translate(matrix, -cam->position);

	return matrix;
}

glm::mat4 Math::CreateModelMatrix(Entity& entity)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, entity.position);

	matrix = glm::rotate(matrix, glm::radians(entity.rotation.x), { 1,0,0 });
	matrix = glm::rotate(matrix, glm::radians(entity.rotation.y), { 0,1,0 });
	matrix = glm::rotate(matrix, glm::radians(entity.rotation.z), { 0,0,1 });

	return matrix;
}

glm::mat4 Math::CreateProjMatrix()
{
	return glm::perspective(glm::radians(85.0f), (float)16 / (float)9, 0.01f, 1000.0f);
}
