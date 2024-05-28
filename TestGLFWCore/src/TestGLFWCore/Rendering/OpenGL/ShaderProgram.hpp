#pragma once
#ifndef _SHADER_PROGRAM_HPP
#define _SHADER_PROGRAM_HPP


#include <glm/mat4x4.hpp>


namespace TestGLFW
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator=(ShaderProgram&&) noexcept;
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool is_compiled() const { return m_isCompiled; }
		void set_matrix4(const char* name, const glm::mat4 matrix) const;
		void set_int(const char* name, const int value) const;

	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}


#endif // _SHADER_PROGRAM_HPP