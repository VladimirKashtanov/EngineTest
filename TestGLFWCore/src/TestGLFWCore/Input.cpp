#include "TestGLFWCore/Input.hpp"


namespace TestGLFW
{
	bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
	bool Input::m_mouse_buttons_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};


	bool Input::isKeyPressed(const KeyCode key_code)
	{
		return m_keys_pressed[static_cast<size_t>(key_code)];
	}

	void Input::pressKey(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = true;
	}

	void Input::releaseKey(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = false;
	}


	bool Input::isMouseButtonPressed(const MouseButton mouse_button)
	{
		return m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)];
	}


	void Input::pressMouseButton(const MouseButton mouse_button)
	{
		m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = true;
	}


	void Input::releaseMouseButton(const MouseButton mouse_button)
	{
		m_mouse_buttons_pressed[static_cast<size_t>(mouse_button)] = false;
	}
}