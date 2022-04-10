#pragma once
#include <ostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Shaders
{
	std::vector<unsigned> shaders, programs;

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
	
	unsigned int loadShaders(const bool src, const std::vector<unsigned int>& types, const std::vector<std::string>& srcs);
	const char* getProgramLog(const unsigned int);
	const char* getShaderLog(const unsigned int);



	class Program
	{
	public:
		// the program ID
		unsigned int ID;

		std::string vertexPath, fragmentPath;

		// constructor reads and builds the shader
		Program(std::string vertexPath, std::string fragmentPath);
		// use/activate the shader
		void use();
		// utility uniform functions
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	};

};