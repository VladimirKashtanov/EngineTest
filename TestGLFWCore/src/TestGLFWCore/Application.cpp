#include "TestGLFWCore/Application.hpp"
#include "TestGLFWCore/Log.hpp" 
#include "TestGLFWCore/Window.hpp"
#include "TestGLFWCore/Event.hpp"
#include "TestGLFWCore/Input.hpp"

#include "TestGLFWCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "TestGLFWCore/Rendering/OpenGL/VertexArray.hpp"
#include "TestGLFWCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "TestGLFWCore/Rendering/OpenGL/Texture2D.hpp"
#include "TestGLFWCore/Camera.hpp"
#include "TestGLFWCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "TestGLFWCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include <GLFW/glfw3.h>

#include <iostream>


namespace TestGLFW
{
	GLfloat positions_coords[] = {
		// front
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,

		// back
		 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,	0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1, // front
		4, 5, 6, 7, 6, 5, // back
		0, 4, 6, 0, 2, 6, // right
		1, 5, 3, 3, 7, 5, // left
		3, 7, 2, 7, 6, 2, // top
		1, 5, 0, 5, 0, 4  // bottom
	};


	void generate_circle(
		unsigned char* data,
		const unsigned int width,
		const unsigned int height,
		const unsigned int center_x,
		const unsigned int center_y,
		const unsigned int radius,
		const unsigned char color_r,
		const unsigned char color_g, 
		const unsigned char color_b)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
				{
					data[3 * (x + width * y) + 0] = color_r;
					data[3 * (x + width * y) + 1] = color_g;
					data[3 * (x + width * y) + 2] = color_b;
				}
			}
		}
	}


	void generate_smile_texture(unsigned char* data, const unsigned int width, const unsigned int height)
	{
		// background
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				data[3 * (x + width * y) + 0] = 200;
				data[3 * (x + width * y) + 1] = 191;
				data[3 * (x + width * y) + 2] = 231;
			}
		}

		// face
		generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

		// smile
		generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
		generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

		// eyes
		generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
		generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);	
	}


	void generate_quards_texture(
		unsigned char* data,
		const unsigned int width,
		const unsigned int height
	)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if ((x < width / 2 && y < height / 2) || (x >= width / 2 && y >= height / 2))
				{
					data[3 * (x + width * y) + 0] = 0;
					data[3 * (x + width * y) + 1] = 0;
					data[3 * (x + width * y) + 2] = 0;
				}
				else
				{
					data[3 * (x + width * y) + 0] = 255;
					data[3 * (x + width * y) + 1] = 255;
					data[3 * (x + width * y) + 2] = 255;
				}
			}
		}
	}


	// вершинный шейдер
	const char* vertex_shader = R"(
		#version 460

		layout(location = 0) in vec3 vertex_position;
		layout(location = 1) in vec2 texture_coord;

		uniform mat4 model_matrix;
		uniform mat4 view_projection_matrix;
		uniform int current_frame;

		out vec2 tex_coord_smile;
		out vec2 tex_coord_quards;

		void main()
		{
			tex_coord_smile = texture_coord;
			tex_coord_quards = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
			gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
		}
	)";

	// фрагментный шейдер
	const char* fragment_shader = R"(
		#version 460

		in vec2 tex_coord_smile;
		in vec2 tex_coord_quards;

		layout(binding = 0) uniform sampler2D InTexture_smile;
		layout(binding = 1) uniform sampler2D InTexture_quards;

		out vec4 frag_color;

		void main()
		{
			frag_color = texture(InTexture_smile, tex_coord_smile) * texture(InTexture_quards, tex_coord_quards);
		}
	)";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<VertexBuffer>  p_cube_positions_vbo;
	std::unique_ptr<IndexBuffer>   p_cube_index_buffer;
	std::unique_ptr<Texture2D>     p_texture_smile;
	std::unique_ptr<Texture2D>     p_texture_quards;
	std::unique_ptr<VertexArray>   p_vao;
	glm::vec3 scale(1.0f, 1.0f, 1.0f);
	float rotate = 0.0f;
	glm::vec3 translate(0.0f, 0.0f, 1.0f);
	float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.0f };

	std::array<glm::vec3, 5> positions = {
		glm::vec3(-2.0f, -2.0f, -4.0f),
		glm::vec3(-5.0f,  0.0f,  3.0f),
		glm::vec3( 2.0f,  1.0f, -2.0f),
		glm::vec3( 4.0f, -3.0f,  3.0f),
		glm::vec3( 1.0f, -7.0f,  1.0f)
	};


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

		m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
			[&](EventMouseButtonPressed& event)
			{
				LOG_INFO("[Mouse button pressed]: {0} at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
				Input::pressMouseButton(event.mouse_button);
				on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
			[&](EventMouseButtonReleased& event)
			{
				LOG_INFO("[Mouse button released]: {0} at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
				Input::releaseMouseButton(event.mouse_button);
				on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyPressed>(
			[&](EventKeyPressed& event)
			{
				if (event.key_code <= KeyCode::KEY_Z)
				{
					if (event.repeated)
					{
						LOG_INFO("[Key pressed]: {0}, repeated", static_cast<char>(event.key_code));
					}
					else
					{
						LOG_INFO("[Key pressed]: {0}", static_cast<char>(event.key_code));
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
					LOG_INFO("[Key released]: {0}", static_cast<char>(event.key_code));
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


		const unsigned int width = 100;
		const unsigned int height = 100;
		const unsigned int channels = 3;
		auto* data = new unsigned char[width * height * channels];

		generate_smile_texture(data, width, height);
		p_texture_smile = std::make_unique<Texture2D>(data, width, height);
		p_texture_smile->bind(0);

		generate_quards_texture(data, width, height);
		p_texture_quards = std::make_unique<Texture2D>(data, width, height);
		p_texture_quards->bind(1);
		
		delete[] data;


		// ------------------------------------------ //
		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->is_compiled())
		{
			return false;
		}

		BufferLayout buffer_layout_vec3_vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float2
		};

		p_vao = std::make_unique<VertexArray>();
		p_cube_positions_vbo = std::make_unique<VertexBuffer>(positions_coords, sizeof(positions_coords), buffer_layout_vec3_vec2);
		p_cube_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_vao->add_vertex_buffer(*p_cube_positions_vbo);
		p_vao->set_index_buffer(*p_cube_index_buffer);
		// ------------------------------------------ //


		static int current_frame = 0;

		// цикл рендеринга
		Renderer_OpenGL::enable_depth_testing();
		while (!m_bCloseWindow)
		{
			Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
			Renderer_OpenGL::clear();

			p_shader_program->bind();

			glm::mat4 model_matrix(1.0f);

			model_matrix = glm::translate(model_matrix, translate);
			
			float rotate_in_rad = glm::radians(rotate);
			model_matrix = glm::rotate(model_matrix, rotate_in_rad, glm::vec3(0, 0, 1));

			model_matrix = glm::scale(model_matrix, scale);

			p_shader_program->set_matrix4("model_matrix", model_matrix);	
			//p_shader_program->set_int("current_frame", current_frame++);

			camera.set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			p_shader_program->set_matrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());
			Renderer_OpenGL::draw(*p_vao);

			for (const glm::vec3& current_position : positions)
			{
				model_matrix = glm::mat4(1.0f);
				model_matrix = glm::translate(model_matrix, current_position);
				p_shader_program->set_matrix4("model_matrix", model_matrix);

				Renderer_OpenGL::draw(*p_vao);
			}

			// ------------------------------------------ //
			UIModule::on_ui_draw_begin();

			bool show = true;
			UIModule::ShowExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();

			ImGui::Begin("Background Color Window");
			ImGui::ColorEdit4("Background Color", m_background_color);
			ImGui::SliderFloat3("scale", glm::value_ptr(scale), 0.0f, 2.0f);
			ImGui::SliderFloat("rotate", &rotate, 0.0f, 360.0f);
			ImGui::SliderFloat3("translate", glm::value_ptr(translate), -0.5f, 0.5f);
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


	glm::vec2 Application::get_current_cursor_position() const
	{
		return m_pWindow->get_current_cursor_position();
	}
}