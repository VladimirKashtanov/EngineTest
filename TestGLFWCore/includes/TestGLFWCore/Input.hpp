#pragma once
#ifndef _INPUT_HPP
#define _INPUT_HPP


#include "Keys.hpp"


namespace TestGLFW
{
	class Input
	{
	public:
		static bool isKeyPressed(const KeyCode key_code);
		static void pressKey(const KeyCode key_code);
		static void releaseKey(const KeyCode key_code);

		static bool isMouseButtonPressed(const MouseButton mouse_button);
		static void pressMouseButton(const MouseButton mouse_button);
		static void releaseMouseButton(const MouseButton mouse_button);

	private:
		static bool m_keys_pressed[];
		static bool m_mouse_buttons_pressed[];
	};
}


#endif // _INPUT_HPP