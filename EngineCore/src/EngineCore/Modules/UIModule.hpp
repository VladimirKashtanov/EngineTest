#pragma once
#ifndef _UI_MODULE_HPP
#define _UI_MODULE_HPP


struct GLFWwindow;


namespace Engine
{
	class UIModule
	{
	public:
		static void on_window_create(GLFWwindow* pWindow);
		static void on_window_close();
		static void on_ui_draw_begin();
		static void on_ui_draw_end();
	};
}


#endif // _UI_MODULE_HPP