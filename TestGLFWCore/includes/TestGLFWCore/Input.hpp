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

	private:
		static bool m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)];
	};
}


#endif // _INPUT_HPP