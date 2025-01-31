#pragma once
#ifndef _RENDERER_OPENGL_HPP
#define _RENDERER_OPENGL_HPP


struct GLFWwindow;

namespace Engine
{
	class VertexArray;

	class Renderer_OpenGL
	{
	public:
		static bool init(GLFWwindow* pWindow);

		static void draw(const VertexArray& vertex_array);
		static void set_clear_color(const float r, const float g, const float b, const float a);
		static void clear();
		static void set_viewport(const unsigned int width, const unsigned int height,
			const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);
		static void enable_depth_testing();
		static void disable_depth_testing();

		static const char* get_vendor_str();
		static const char* get_renderer_str();
		static const char* get_version_str();
	};
}


#endif // _RENDERER_OPENGL_HPP