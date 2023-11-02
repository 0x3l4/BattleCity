#pragma once

#include <string>
#include <glad/glad.h>

namespace Renderer
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) noexcept;
		ShaderProgram& operator =(const ShaderProgram&) = delete;
		ShaderProgram& operator =(ShaderProgram&&) noexcept;
		~ShaderProgram();
		bool isCompiled() const;
		void use();

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = 0;
		GLuint m_ID = 0;

	};
}