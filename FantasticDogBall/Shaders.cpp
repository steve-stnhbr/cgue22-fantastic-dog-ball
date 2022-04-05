#include "Shaders.h"
#include "Utils.h"

unsigned int Shaders::shaderSource(unsigned type, const std::string& src)
{
	unsigned int id = glCreateShader(type);
	const char* cSrc = src.c_str();
	glShaderSource(id, 1, &cSrc, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = static_cast<char*>(_malloca(length*sizeof(char)));
		glGetShaderInfoLog(id, length, nullptr, message);
		throw ShaderCompilationException(message, src.c_str());
	}

	return id;
}

unsigned Shaders::shaderFile(unsigned type, const std::string& src)
{
	return Shaders::shaderSource(type, Utils::readFile(src.c_str()));
}


unsigned int Shaders::loadShadersFile(const std::vector<unsigned> types, const std::vector<std::string> srcs)
{
	return loadShaders(false, types, srcs);
}

unsigned int Shaders::loadShadersSource(const std::vector<unsigned> types, const std::vector<std::string> srcs)
{
	return loadShaders(true, types, srcs);
}


unsigned int Shaders::loadShaders(bool src, const std::vector<unsigned> types, const std::vector<std::string> srcs)
{
	const auto size = types.size();
	if (size != srcs.size())
		throw std::invalid_argument("the sizes of types and src-strings is not equal");

	unsigned int program = glCreateProgram();

	for (auto i = 0; i < size; i++)
	{
		const unsigned int shader = src ? Shaders::shaderSource(types[i], srcs[i]) : Shaders::shaderFile(types[i], srcs[i]);
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		throw ProgramLinkException(message, program);
	}

	return program;
}

Shaders::ProgramLinkException::ProgramLinkException() : base(""), program(0)
{}
Shaders::ProgramLinkException::ProgramLinkException(const char* message) : base(message), program(0)
{}
Shaders::ProgramLinkException::ProgramLinkException(const char* const message, int _program): base(message), program(_program)
{}

Shaders::ShaderCompilationException::ShaderCompilationException() : base("")
{}
Shaders::ShaderCompilationException::ShaderCompilationException(const char* message) : base(message)
{}
Shaders::ShaderCompilationException::ShaderCompilationException(const char* message, const char* _shaderName): base(message), shaderName(_shaderName)
{}

