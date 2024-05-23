#pragma once
#ifndef _WINDOW_HPP
#define _WINDOW_HPP


#include <TestGLFWCore/Event.hpp>

#include <string>
#include <functional>


struct GLFWwindow;


namespace TestGLFW
{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&)      = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&)      = delete;

		void on_update();
		unsigned int get_width()  const { return m_data.width;  }
		unsigned int get_height() const { return m_data.height; }

		void set_event_callback(const EventCallbackFn& callback)
		{
			m_data.eventCallbackFn = callback;
		}

	private:
		struct WindowData
		{
			std::string   title;
			unsigned int  width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shoutdown();

		GLFWwindow* m_pWindow = nullptr;
		WindowData     m_data;
		float m_background_color[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	};
}


#endif // _WINDOW_HPP