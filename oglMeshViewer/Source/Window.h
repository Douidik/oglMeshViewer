#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "../Debug.h"

struct DestroyWindow
{
	void operator()(GLFWwindow* window)
	{
		//@TODO Create GLFW destructor
	}
};

typedef std::unique_ptr<GLFWwindow, DestroyWindow> Window_ptr;

namespace Window
{
	constexpr unsigned int WIDTH = 1280, HEIGHT = 720;

	void Init();

	bool GetIsOpen();

	void Clear();
	void Update();
	void CheckForClose();

	GLFWwindow& Get();

	void Close();
}