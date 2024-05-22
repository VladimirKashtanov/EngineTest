#pragma once
#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP


#include "TestGLFWCore/Application.hpp"
#include <TestGLFWCore/Log.hpp> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace TestGLFW
{
	Application::Application()
	{
        
	}

	Application::~Application()
	{

	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* window_title)
	{
        GLFWwindow* window;

        // ������������� GLFW
        if (!glfwInit())
        {
            LOG_CRITICAL("Failed to initialize GLFW!");
            return -1;
        }

        // �������� ������� ����
        window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);


        // ������������� GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD!");
            return -1;
        }


        // ��������� ����� ������� ������ �����
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

        // ���� ����������
        while (!glfwWindowShouldClose(window))
        {
            // ���������

            // ������� ������ �����
            glClear(GL_COLOR_BUFFER_BIT);

            // ����� ����������� ������� � ��������� �������
            glfwSwapBuffers(window);
            glfwPollEvents();

            // ���������� ������
            on_update();
        }

        // ������������ �������� 
        glfwTerminate();
        return 0;
	}
}


#endif // _APPLICATION_HPP