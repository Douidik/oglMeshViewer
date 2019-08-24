#include "Camera.h"

void Camera::input(float dt)
{
	glm::vec3 movement = glm::vec3(0.0f);
	float speed = 2;

	//Forward
	if (glfwGetKey(&Window::Get(), GLFW_KEY_W) == GLFW_PRESS)
	{
		movement.x -= cos(glm::radians(rotation.y + 90)) * speed;
		movement.z -= sin(glm::radians(rotation.y + 90)) * speed;
	}
	//Backward
	if (glfwGetKey(&Window::Get(), GLFW_KEY_S) == GLFW_PRESS)
	{
		movement.x += cos(glm::radians(rotation.y + 90)) * speed;
		movement.z += sin(glm::radians(rotation.y + 90)) * speed;
	}
	//Left strafe
	if (glfwGetKey(&Window::Get(), GLFW_KEY_A) == GLFW_PRESS)
	{
		movement.x += -cos(glm::radians(rotation.y)) * speed;
		movement.z += -sin(glm::radians(rotation.y)) * speed;
	}
	//Right strafe
	if (glfwGetKey(&Window::Get(), GLFW_KEY_D) == GLFW_PRESS)
	{
		movement.x += cos(glm::radians(rotation.y)) * speed;
		movement.z += sin(glm::radians(rotation.y)) * speed;
	}


	//up
	if (glfwGetKey(&Window::Get(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		movement.y += speed * 2;
	}
	//down
	if (glfwGetKey(&Window::Get(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		movement.y -= speed * 2;
	}

	position += movement * dt;
		
	if (glfwGetMouseButton(&Window::Get(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		mouseInput();
}

void Camera::mouseInput()
{
	double mouseX, mouseY;
	glfwSetInputMode(&Window::Get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwGetCursorPos(&Window::Get(), &mouseX, &mouseY);

	glm::vec2 mouseMovement = glm::vec2{ mouseX - lastMousePosX,
										 mouseY - lastMousePosY };

	rotation.y += mouseMovement.x * static_cast<float>(0.1);
	rotation.x += mouseMovement.y * static_cast<float>(0.1);

	// Pitch
	if (rotation.x > 80)
	{
		rotation.x = 80;
	}
	else if (rotation.x < -80)
	{
		rotation.x = -80;
	}
	//Yaw
	if (rotation.y < 0)
	{
		rotation.y = 360;
	}
	else if (rotation.y > 360)
	{
		rotation.y = 0;
	}

	float centerX = Window::WIDTH / 2;
	float centerY = Window::HEIGHT / 2;

	glfwSetCursorPos(&Window::Get(), centerX, centerY);
							  
	glfwGetCursorPos(&Window::Get(), &lastMousePosX, &lastMousePosY);
}
