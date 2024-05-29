#pragma once
#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP


#include "EngineCore/Event.hpp"
#include "EngineCore/Camera.hpp"

#include <memory>


namespace Engine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&)      = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&)      = delete;

		virtual int start(unsigned int window_width, unsigned int window_height, const char* window_title);
		void close();

		virtual void on_update() {}

		virtual void on_ui_draw() {}

		virtual void on_mouse_button_event(
			const MouseButton button_code, 
			const double x_pos, 
			const double y_pos, 
			const bool pressed) 
		{}

		glm::vec2 get_current_cursor_position() const;

		Camera camera{ glm::vec3(-5.0f, 0.0f, 0.0f) };

		glm::vec3 light_source_position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 light_source_color{ 1.0f, 1.0f, 1.0f };
		float ambient_factor  = 0.1f;
		float diffuse_factor  = 1.0f;
		float specular_factor = 0.5f;
		float shininess = 32.0f;

	private:
		void draw();

		std::unique_ptr<class Window> m_pWindow;

		EventDispatcher m_event_dispatcher;
		bool        m_bCloseWindow = false;
	};
}


#endif // _APPLICATION_HPP