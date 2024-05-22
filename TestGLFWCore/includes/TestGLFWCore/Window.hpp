#pragma once
#ifndef _WINDOW_HPP
#define _WINDOW_HPP


#include <string>


struct GLFWwindow;


namespace TestGLFW
{
	class Window
	{
	public:
		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&)      = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&)      = delete;

		void on_update();
		unsigned int get_width()  const { return m_width;  }
		unsigned int get_height() const { return m_height; }

	private:
		int init();
		void shoutdown();

		GLFWwindow* m_pWindow;
		std::string   m_title;
		unsigned int  m_width;
		unsigned int m_height;
	};
}


#endif // _WINDOW_HPP