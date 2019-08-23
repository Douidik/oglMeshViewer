#include "Window.h"

namespace Window {

	bool isOpen;
	Window_ptr window;

	void Init()
	{
		if (!glfwInit())
		{
			std::cerr << "Can't initialize GLFW." << std::endl;
			return;
		}
		
		glfwWindowHint(GLFW_SAMPLES, 4);

		/*
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		*/

		window.reset(glfwCreateWindow(WIDTH, HEIGHT, "Model Viewer", nullptr, nullptr));
		glfwMakeContextCurrent(&Get());
		
		isOpen = ((window) ? true : false);
		
		glewExperimental = true;
		glewInit();
		
		glViewport(0, 0, WIDTH, HEIGHT);

		//Depth func
		glcall(glEnable(GL_DEPTH_TEST));
		glcall(glDepthFunc(GL_LESS));

		//Cull facing
		glcall(glEnable(GL_CULL_FACE));
		glcall(glCullFace(GL_FRONT));
		glcall(glFrontFace(GL_CW));

		glfwSetInputMode(&Get(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	bool GetIsOpen()
	{
		return isOpen;
	}

	void Clear()
	{
		glcall(glClearColor(0, 0, 1, 0));
		glcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Update()
	{
		glfwSwapBuffers(&Get());
		glfwPollEvents();
	}

	void CheckForClose()
	{
		if (glfwWindowShouldClose(&Get()) ||
			glfwGetKey(&Get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			Close();
		}
	}

	GLFWwindow& Get()
	{
		return *window.get();
	}

	void Close()
	{
		isOpen = false;
		glfwDestroyWindow(&Get());
		exit(EXIT_SUCCESS);
	}

}