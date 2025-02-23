#pragma once
#include <atomic>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "UniformBuffer.h"
#include "Texture.h"
#include "Loggger.h"

namespace Shaders
{

	void cleanup();
	unsigned int shaderFile(unsigned int type, const std::string& src);
	unsigned int shaderSource(unsigned int type, const std::string& src);
	unsigned int loadShadersFile(const std::vector<unsigned int>& types, const std::vector<std::string>& srcs);
	unsigned int loadShadersSource(const std::vector<unsigned int>& types, const std::vector<std::string>& srcs);

	class ShaderCompilationException: public std::logic_error
	{
	public:
		using base = std::logic_error;

		std::string shaderName;
		ShaderCompilationException();
		ShaderCompilationException(const char* message);
		ShaderCompilationException(const char* message, const char* shaderName);
	};

	class ProgramLinkException: public std::logic_error
	{
	public:
		using base = std::logic_error;
		int program;
		ProgramLinkException();
		ProgramLinkException(const char* const message);
		ProgramLinkException(const char* const message, int program);
	};

	class ShaderFileExtensionException : public std::logic_error
	{
	public:
		using base = std::logic_error;
		std::string file, extension;
		ShaderFileExtensionException();
		ShaderFileExtensionException(const char* const message);
		ShaderFileExtensionException(const char* const message, const char* const file, const char* const extension);
	};
	
	unsigned int loadShaders(const bool src, const std::vector<unsigned int>& types, const std::vector<std::string>& srcs);
	const char* getProgramLog(const unsigned int);
	const char* getShaderLog(const unsigned int);

	class Program
	{
	public:
		// the program ID
		unsigned int ID;
		bool linked = false;
		int binding;
		int location;

		std::vector<std::string> paths;
		std::map<std::string, std::pair<unsigned, unsigned>> buffer_map;
		std::map<std::string, std::pair<unsigned, unsigned>> texture_map;

		std::string vertexPath, fragmentPath;

		Program();
		// constructor reads and builds the shader
		Program(std::string& vertexPath, std::string& fragmentPath);
		Program(std::vector<std::string> paths);
		// use/activate the shader
		void use() const;
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, glm::vec3) const;
		void setVector4(const std::string& name, glm::vec4) const;
		void setMatrix4(const std::string& name, glm::mat4) const;
		void setUniform(const std::string& name, UncheckedUniformBuffer buffer);
		void setUniform(const int binding, UncheckedUniformBuffer buffer) const;
		bool checkLocation(unsigned location, std::string name) const;
		template <typename V>
		void setUniform(const std::string& name, UniformBuffer<V> buffer)
		{
			auto binding_ = buffer.id + binding;
			Loggger::debug("Binding uniform %s to program %d", name.c_str(), ID);
			const auto ubi = glGetUniformBlockIndex(ID, name.c_str());
			buffer.bind(binding_);
			glUniformBlockBinding(ID, ubi, binding_);
		}

		void setTexture(const unsigned, const Texture::Texture& texture) const;
		void setTexture(const std::string&, const Texture::Texture& texture);


		std::string getLog();
	};
};