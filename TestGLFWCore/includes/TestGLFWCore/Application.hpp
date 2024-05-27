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

		virtual void on_update() { }

		virtual void on_ui_draw() { }

		float camera_position[3] = { 0.0f, 0.0f, 2.0f };
		float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
		bool perspective_camera = true;
		Camera camera;

	private:
		std::unique_ptr<struct Window> m_pWindow;

		EventDispatcher m_event_dispatcher;
		bool        m_bCloseWindow = false;
	};
}


#endif // _APPLICATION_HPP