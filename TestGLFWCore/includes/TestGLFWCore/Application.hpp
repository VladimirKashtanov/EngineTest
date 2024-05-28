#pragma once
#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP


#include "TestGLFWCore/Event.hpp"
#include "TestGLFWCore/Camera.hpp"

#include <memory>


namespace TestGLFW
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

		float camera_position[3] = { 0.0f, 0.0f, 2.0f };
		float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
		float camera_fov = 60.f;
		float camera_near_plane = 0.1f;
		float camera_far_plane = 100.0f;
		bool perspective_camera = true;
		Camera camera{ glm::vec3(-5.0f, 0.0f, 0.0f) };

	private:
		void draw();

		std::unique_ptr<class Window> m_pWindow;

		EventDispatcher m_event_dispatcher;
		bool        m_bCloseWindow = false;
	};
}


#endif // _APPLICATION_HPP