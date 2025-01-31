#include "ShaderProgram.hpp"

#include "EngineCore/Log.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>	


namespace Engine
{
	bool create_shader(const char* source, const GLenum shader_type, GLuint& shader_id)
	{
		shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		GLint success;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			char info_log[1024];
			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);

			LOG_CRITICAL("Failed to compile shader:\n{0}", info_log);
			return false;
		}
		return true;
	}


	ShaderProgram::ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src)
	{
		// ���������� ���������� �������
		GLuint vertex_shader_id = 0;
		if (!create_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id))
		{
			LOG_CRITICAL("VERTEX_SHADER: compile-time error!");
			glDeleteShader(vertex_shader_id);
			return;
		}

		// ���������� ������������ �������
		GLuint fragment_shader_id = 0;
		if (!create_shader(fragment_shader_src, GL_FRAGMENT_SHADER, fragment_shader_id))
		{
			LOG_CRITICAL("FRAGMENT_SHADER: compile-time error!");
			glDeleteShader(fragment_shader_id);
			return;
		}

		// ���������� ��������� ���������
		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar info_log[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
			LOG_CRITICAL("SHADER_PROGRAM: link-time error!:\n{0}", info_log);
			glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertex_shader_id);
			glDeleteShader(fragment_shader_id);
			return;
		}
		else {
			m_isCompiled = true;
		}

		glDetachShader(m_id, vertex_shader_id);
		glDetachShader(m_id, fragment_shader_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
	}


	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}


	void ShaderProgram::bind() const
	{
		glUseProgram(m_id);
	}


	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}


	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_id);
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}


	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_isCompiled = false;
	}


	void ShaderProgram::set_matrix4(const char* name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void ShaderProgram::set_matrix3(const char* name, const glm::mat3& matrix) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void ShaderProgram::set_int(const char* name, const int value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name), value);
	}


	void ShaderProgram::set_vec3(const char* name, const glm::vec3& value) const
	{
		glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
	}


	void ShaderProgram::set_float(const char* name, const float value) const
	{
		glUniform1f(glGetUniformLocation(m_id, name), value);
	}
}