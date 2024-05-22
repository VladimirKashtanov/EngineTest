#pragma once
#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP


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

	private:
		std::unique_ptr<struct Window> m_pWindow;
	};
}


#endif // _APPLICATION_HPP