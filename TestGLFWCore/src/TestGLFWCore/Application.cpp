#include "TestGLFWCore/Application.hpp"
#include <TestGLFWCore/Log.hpp> 
#include <TestGLFWCore/Window.hpp>

#include <iostream>


namespace TestGLFW
{
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

		// цикл рендеринга
		while (true)
		{
			m_pWindow->on_update();
			on_update();
		}

        return 0;
	}
}