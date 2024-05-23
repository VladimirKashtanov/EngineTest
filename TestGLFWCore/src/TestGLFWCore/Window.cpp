#include "TestGLFWCore/Window.hpp"
#include "TestGLFWCore/Log.hpp"
#include "TestGLFWCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexArray.hpp"
#include "TestGLFWCore/Rendering/OpenGL/IndexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glm/mat3x3.hpp>


namespace TestGLFW
{
	static bool s_GLFW_initialize = false;

	GLfloat positions_colors[] = {
		-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
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
	std::unique_ptr<VertexBuffer>  p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer>   p_index_buffer;
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

		BufferLayout buffer_layout_2vec3
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		p_vao = std::make_unique<VertexArray>();
		p_positions_colors_vbo = std::make_unique<VertexBuffer>(
			positions_colors,
			sizeof(positions_colors),
			buffer_layout_2vec3
		);

		p_index_buffer = std::make_unique<IndexBuffer>(
			indices,
			sizeof(indices) / sizeof(GLuint)
		);

		p_vao->add_vertex_buffer(*p_positions_colors_vbo);
		p_vao->set_index_buffer(*p_index_buffer);


		glm::mat3 mat_1(4, 0, 0, 2, 8, 1, 0, 1, 0);
		glm::mat3 mat_2(4, 2, 9, 2, 0, 4, 1, 4, 2);
		glm::mat3 res_mat = mat_1 * mat_2;

		LOG_INFO("");
		LOG_INFO("|{0:3} {1:3} {2:3}|", res_mat[0][0], res_mat[1][0], res_mat[2][0]);
		LOG_INFO("|{0:3} {1:3} {2:3}|", res_mat[0][1], res_mat[1][1], res_mat[2][1]);
		LOG_INFO("|{0:3} {1:3} {2:3}|", res_mat[0][2], res_mat[1][2], res_mat[2][2]);
		LOG_INFO("");

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


		// окно demo ImGui
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(get_width());
		io.DisplaySize.y = static_cast<float>(get_height());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui::ShowDemoWindow();


		// виджет дл€ выбора цвета заливки фона окна opengl
		ImGui::Begin("Background Color Window");
		ImGui::ColorEdit4("Background Color", m_background_color);

		// треугольник
		p_shader_program->bind();
		p_vao->bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr);

		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}