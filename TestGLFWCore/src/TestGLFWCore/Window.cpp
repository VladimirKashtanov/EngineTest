#include "TestGLFWCore/Window.hpp"
#include "TestGLFWCore/Log.hpp"
#include "TestGLFWCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexArray.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


namespace TestGLFW
{
	static bool s_GLFW_initialize = false;

	// координаты вершин треугольника
	GLfloat points[] = {
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	// цвета вершин треугольника
	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	// вершинный шейдер
	const char* vertex_shader =
		"#version 460\n"
		"layout(location = 0) in vec3 vertex_position;\n"
		"layout(location = 1) in vec3 vertex_color;\n"
		"out vec3 color;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(vertex_position, 1.0);\n"
		"	color = vertex_color;\n"
		"}\0";

	// фрагментный шейдер
	const char* fragment_shader =
		"#version 460\n"
		"in vec3 color;\n"
		"out vec4 frag_color;\n"
		"void main()\n"
		"{\n"
		"	frag_color = vec4(color, 1.0);\n"
		"}\0";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<VertexBuffer>  p_points_vbo;
	std::unique_ptr<VertexBuffer>  p_colors_vbo;
	std::unique_ptr<VertexArray>   p_vao;

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: m_data({ std::move(title), width, height })
	{
		int resultCode = init();

		// »нициализаци€ ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	}


	Window::~Window()
	{
		shoutdown();
	}


	int Window::init()
	{
		LOG_INFO("Creating window '{0}' with size {1}x{2}", m_data.title, m_data.width, m_data.height);

		// инициализаци€ GLFW
		if (!s_GLFW_initialize)
		{
			if (!glfwInit())
			{
				LOG_CRITICAL("Failed to initialize GLFW!");
				return -1;
			}
			s_GLFW_initialize = true;
		}

		// создание объекта окна
		m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		if (!m_pWindow)
		{
			LOG_CRITICAL("Failed to create window '{0}' with size {1}x{2}", m_data.title, m_data.width, m_data.height);
			glfwTerminate();
			return -2;
		}
		glfwMakeContextCurrent(m_pWindow);

		// »нициализаци€ GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_CRITICAL("Failed to initialize GLAD!");
			return -3;
		}

		glfwSetWindowUserPointer(m_pWindow, &m_data);

		glfwSetWindowSizeCallback(m_pWindow,
			[](GLFWwindow* pWindow, int width, int height)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				data.width  = width;
				data.height = height;

				EventWindowResize event(width, height);
				data.eventCallbackFn(event);
			});

		glfwSetCursorPosCallback(m_pWindow,
			[](GLFWwindow* pWindow, double x, double y)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

				EventMouseMoved event(x, y);
				data.eventCallbackFn(event);
			});

		glfwSetWindowCloseCallback(m_pWindow,
			[](GLFWwindow* pWindow)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

				EventWindowClose event;
				data.eventCallbackFn(event);
			});

		glfwSetFramebufferSizeCallback(m_pWindow,
			[](GLFWwindow* pWindow, int width, int height)
			{
				glViewport(0, 0, width, height);
			});


		// компил€ци€ шейдерной программы
		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->isCompiled())
		{
			return false;
		}

		p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points));
		p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors));
		p_vao = std::make_unique<VertexArray>();

		p_vao->add_buffer(*p_points_vbo);
		p_vao->add_buffer(*p_colors_vbo);

		return 0;
	}


	void Window::shoutdown()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}


	void Window::on_update()
	{
		glClearColor(m_background_color[0],
			m_background_color[1],
			m_background_color[2],
			m_background_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);


		// треугольник
		p_shader_program->bind();
		p_vao->bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// окно demo ImGui
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();


		// виджет дл€ выбора цвета заливки фона окна opengl
		ImGui::Begin("Background Color Window");
		ImGui::ColorEdit4("Background Color", m_background_color);
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}