#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer
{
	class ShaderProgram;
}


class ResourceManager
{
public:
	ResourceManager(const std::string& executablePath);
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
	~ResourceManager() = default;

	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string vertexPath, const std::string fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);
	

private:
	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderPrograms;
	ShaderPrograms m_shaderPrograms;

	std::string m_path;
};
