#pragma once
#ifndef _VERTEX_AARAY_HPP
#define _VERTEX_AARAY_HPP


#include "VertexBuffer.hpp"


namespace TestGLFW
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& vertex_array) noexcept;
		VertexArray(VertexArray&& vertex_array) noexcept;

		void add_buffer(const VertexBuffer& vertex_buffer);
		void bind() const;
		static void unbind();

	private:
		unsigned int m_id = 0;
		unsigned int m_elements_count = 0;
	};
}


#endif // _VERTEX_AARAY_HPP