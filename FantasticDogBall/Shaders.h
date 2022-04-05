#pragma once
#include <ostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Shaders
{
public:
	static unsigned int shaderFile(unsigned int type, const std::string& src);
	static unsigned int shaderSource(unsigned int type, const std::string& src);
	static unsigned int loadShadersFile(const std::vector<unsigned int> types, const std::vector<std::string> srcs);
	static unsigned int loadShadersSource(const std::vector<unsigned int> types, const std::vector<std::string> srcs);


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

private:
	static unsigned int loadShaders(bool src, const std::vector<unsigned int> types, const std::vector<std::string> srcs);
};

