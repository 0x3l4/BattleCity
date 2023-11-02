#include "ShaderProgram.h"
#include <iostream>

Renderer::ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint vertexShaderID;
	if (createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID) == false)
	{
		std::cerr << "VERTEX SHADER: Compile-time error" << std::endl;
		return;
	}
	
	GLuint fragmentShaderID;
	if (createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID) == false)
	{
		std::cerr << "FRAGMENT SHADER: Compile-time error" << std::endl;
		glDeleteShader(vertexShaderID);
		return;
	}

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, fragmentShaderID);
	glLinkProgram(m_ID);

	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		const GLsizei bufSize = 1024;
		GLchar infoLog[bufSize];
		glGetShaderInfoLog(m_ID, bufSize, nullptr, infoLog);

		std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog;
	}
	else
	{
		m_isCompiled = true;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Renderer::ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
{
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;
	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

Renderer::ShaderProgram& Renderer::ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
{
	glDeleteProgram(m_ID);

	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;
	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;

	return *this;
}

Renderer::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

bool Renderer::ShaderProgram::isCompiled() const
{
	return m_isCompiled;
}

void Renderer::ShaderProgram::use()
{
	glUseProgram(m_ID);
}

bool Renderer::ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
{
	shaderID = glCreateShader(shaderType);
	const char* code = source.c_str();
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		const GLsizei bufSize = 1024;
		GLchar infoLog[bufSize];
		glGetShaderInfoLog(shaderID, bufSize, nullptr, infoLog);

		std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog;

		return false;
	}

	return true;
}
