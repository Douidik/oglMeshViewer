#include "Application.h"

Application::Application()
	:m_shaderPhong(Shader("Res/Shaders/Common.vert", "Res/Shaders/Common.frag")),
	m_shaderNoLight(Shader("Res/Shaders/NoLight.vert", "Res/Shaders/NoLight.frag")),
	m_model(Model("Res/Models/Nanosuit/nanosuit.obj")),
	 m_camera(Camera())
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(&Window::Get(), true);
	ImGui::StyleColorsDark();
	
	m_shader = &m_shaderPhong;
}

void Application::Update()
{
	glm::vec3 lightPos(4, 3, 3);
	glm::vec3 lightColor(1, 1, 1);

	double dt, lastTime;
	dt = lastTime = 0.0;

	while (Window::GetIsOpen())
	{

		ImGui_ImplGlfwGL3_NewFrame();

		dt = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		//Handle window events
		Window::CheckForClose();		
	
		//Switching between two shaders
		if (glfwGetKey(&Window::Get(), GLFW_KEY_P) == GLFW_PRESS)
		{
			m_shader = &m_shaderPhong;
		}
		if (glfwGetKey(&Window::Get(), GLFW_KEY_L) == GLFW_PRESS)
		{
			m_shader = &m_shaderNoLight;
		}

		m_camera.input(dt);

		glm::mat4 modelMat = Math::CreateModelMatrix(m_model);

		glm::mat4 viewMat = Math::CreateViewMatrix(&m_camera);
		glm::mat4 projMat = Math::CreateProjMatrix();

		modelMat = glm::scale(modelMat, glm::vec3(0.1, 0.1, 0.1));

		m_shader->SetModelMatrix(modelMat);
		m_shader->SetViewMatrix(viewMat);
		m_shader->SetProjMatrix(projMat);

		if (m_shader == &m_shaderPhong)
		{

			m_shader->Set3f("lightPos", lightPos);
			m_shader->Set3f("viewPos", m_camera.position);
			m_shader->Set3f("lightColor", lightColor);

		}

		m_model.Draw(*m_shader);

		//Imgui slides
		{
			ImGui::SliderFloat3("Light position", &lightPos[0], -20.0f, 20.0f);
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


		Window::Update();
		Window::Clear();
	}
}