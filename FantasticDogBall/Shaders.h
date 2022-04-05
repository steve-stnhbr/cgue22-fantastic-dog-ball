#pragma once
#include <ostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Shaders
{
	static unsigned int shader(unsigned int type, const std::string& src)
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

			char message[length];
			glGetShaderInfoLog(id, length, nullptr, message);
			printf("Failed to compile shader %s:\n %s", src.c_str(), message);
		}

		return id;
	}


	static unsigned int loadShaders(const std::vector<unsigned int> types, const std::vector<std::string> srcs)
	{
		auto size = types.size();
		if (size != srcs.size())
			throw new std::invalid_argument("the sizes of types and src-strings is not equal");

		unsigned int program = glCreateProgram();

		for (auto i = 0; i < size; i++)
		{
			unsigned int shader = Shaders::shader(types[i], srcs[i]);
			glAttachShader(program, shader);
			glDeleteShader(shader);
		}

		int result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			char message[length];
			glGetProgramInfoLog(program, length, nullptr, message);
			printf("Failed to link shaders to program:\n%s", message);
		}
	}
};

