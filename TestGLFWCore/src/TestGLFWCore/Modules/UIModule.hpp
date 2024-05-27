#pragma once
#ifndef _UI_MODULE_HPP
#define _UI_MODULE_HPP


struct GLFWwindow;


namespace TestGLFW
{
	class UIModule
	{
	public:
		static void on_window_create(GLFWwindow* pWindow);
		static void on_window_close();
		static void on_ui_draw_begin();
		static void on_ui_draw_end();

		static void ShowExampleAppDockSpace(bool* p_open);
	};
}


#endif // _UI_MODULE_HPP