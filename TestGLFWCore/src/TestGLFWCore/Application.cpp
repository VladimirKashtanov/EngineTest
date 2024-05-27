#include "TestGLFWCore/Application.hpp"
#include "TestGLFWCore/Log.hpp" 
#include "TestGLFWCore/Window.hpp"
#include "TestGLFWCore/Event.hpp"
#include "TestGLFWCore/Input.hpp"

#include "TestGLFWCore/Camera.hpp"
#include "TestGLFWCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexArray.hpp"
#include "TestGLFWCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "TestGLFWCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "TestGLFWCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include <GLFW/glfw3.h>

#include <iostream>


namespace TestGLFW
{
	GLfloat positions_colors[] = {
		0.0f, -0.5f, -0.5f, 		1.0f, 1.0f, 0.0f,
		0.0f,  0.5f, -0.5f, 		0.0f, 1.0f, 1.0f,
		0.0f, -0.5f,  0.5f, 		1.0f, 0.0f, 1.0f,
		0.0f,  0.5f,  0.5f, 		1.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
	};

	// вершинный шейдер
	const char* vertex_shader = R"(
		#version 460
		layout(location = 0) in vec3 vertex_position;
		layout(location = 1) in vec3 vertex_color;
		uniform mat4 model_matrix;
		uniform mat4 view_projection_matrix;
		out vec3 color;
		void main()
		{
			gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
			color = vertex_color;
		}
	)";

	// фрагментный шейдер
	const char* fragment_shader = R"(
		#version 460
		in vec3 color;
		out vec4 frag_color;
		void main()
		{
			frag_color = vec4(color, 1.0);
		}
	)";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<VertexBuffer>  p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer>   p_index_buffer;
	std::unique_ptr<VertexArray>   p_vao;
	float scale[3] = { 1.0f, 1.0f, 1.0f };
	float rotate = 0.0f;
	float translate[3] = { 0.0f, 0.0f, 1.0f };
	float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.0f };


	Application::Application()
	{
		LOG_INFO("Starting Application");
	}

	Application::~Application()
	{
		LOG_INFO("Closing Application");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* window_title)
	{
		m_pWindow = std::make_unique<Window>(window_title, window_width, window_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				//LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event)
			{
				LOG_INFO("[WindowClosed]");
				m_bCloseWindow = true;
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyPressed>(
			[&](EventKeyPressed& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					if (event.repeated)
					{
						LOG_INFO("[Key Pressed]: {0}, repeated", static_cast<char>(event.key_code));
					}
					else
					{
						LOG_INFO("[Key Pressed]: {0}", static_cast<char>(event.key_code));
					}
				}
				Input::pressKey(event.key_code);
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyReleased>(
			[&](EventKeyReleased& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					LOG_INFO("[Key Released]: {0}", static_cast<char>(event.key_code));
				}
				Input::releaseKey(event.key_code);
			}
		);

		m_pWindow->set_event_callback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);


		// ------------------------------------------ //
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
		// ------------------------------------------ //


		// цикл рендеринга
		while (!m_bCloseWindow)
		{
			Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
			Renderer_OpenGL::clear();


			glm::mat4 scale_matrix(scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				0, 0, 0, 1
			);

			float rotate_in_rad = glm::radians(rotate);
			glm::mat4 rotate_matrix(cos(rotate_in_rad), sin(rotate_in_rad), 0, 0,
				-sin(rotate_in_rad), cos(rotate_in_rad), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			
			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate[0], translate[1], translate[2], 1
			);

			glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
			p_shader_program->setMatrix4("model_matrix", model_matrix);

			camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			p_shader_program->setMatrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());


			p_shader_program->bind();

			Renderer_OpenGL::draw(*p_vao);

			// ------------------------------------------ //
			UIModule::on_ui_draw_begin();

			bool show = true;
			UIModule::ShowExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();

			ImGui::Begin("Background Color Window");
			ImGui::ColorEdit4("Background Color", m_background_color);
			ImGui::SliderFloat3("scale", scale, 0.0f, 2.0f);
			ImGui::SliderFloat("rotate", &rotate, 0.0f, 360.0f);
			ImGui::SliderFloat3("translate", translate, -0.5f, 0.5f);
			ImGui::SliderFloat3("camera position", camera_position, -10.0f, 10.0f);
			ImGui::SliderFloat3("camera rotation", camera_rotation, 0.0f, 360.0f);
			ImGui::Checkbox("Perspective camera", &perspective_camera);
			ImGui::End();
			// ------------------------------------------ //

			on_ui_draw();

			UIModule::on_ui_draw_end();

			m_pWindow->on_update();
			on_update();
		}
		m_pWindow = nullptr;

        return 0;
	}
}