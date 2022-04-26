#include "Shaders.h"

#include <iostream>

#include "LightSource.h"
#include "Render.h"

std::vector<unsigned> shaders, programs;


void Shaders::cleanup()
{
	for (const unsigned element : programs)
	{
		glDeleteProgram(element);
	}
	for (const unsigned element : shaders)
	{
		glDeleteShader(element);
	}
}

unsigned int Shaders::shaderSource(unsigned type, const std::string& src)
{
	std::string source = src;
	const unsigned int id = glCreateShader(type);
	if(type == GL_FRAGMENT_SHADER)
	{
		const std::string lightNums =
			"\nconst int NUM_POINT_LIGHTS = " + std::to_string(Light::Light::NUM_POINT_LIGHTS) + ";\n"
			+ "const int NUM_DIRECTIONAL_LIGHTS = " + std::to_string(Light::Light::NUM_DIRECTIONAL_LIGHTS) + ";\n"
			+ "const int NUM_SPOT_LIGHTS = " + std::to_string(Light::Light::NUM_SPOT_LIGHTS) + ";\n";
		source.insert(source.find_first_of("\n"), lightNums);
	}
	const char* cSrc = source.c_str();
	glShaderSource(id, 1, &cSrc, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		throw ShaderCompilationException(getShaderLog(id), source.c_str());
	}

	Loggger::info("Created shader %d", id);

	return id;
}

unsigned Shaders::shaderFile(unsigned type, const std::string& src)
{
	Loggger::info( "Creating shader(%d) from file %s", type, src.c_str());
	return Shaders::shaderSource(type, Utils::readFile(src.c_str()));
}


unsigned int Shaders::loadShadersFile(const std::vector<unsigned>& types, const std::vector<std::string>& srcs)
{
	return loadShaders(false, types, srcs);
}

unsigned int Shaders::loadShadersSource(const std::vector<unsigned>& types, const std::vector<std::string>& srcs)
{
	return loadShaders(true, types, srcs);
}


unsigned int Shaders::loadShaders(const bool src, const std::vector<unsigned>& types, const std::vector<std::string>& srcs)
{
	const auto size = types.size();
	if (size != srcs.size())
		throw std::length_error("the sizes of types and src-strings is not equal");

	const unsigned int program = glCreateProgram();

	if(program == 0)
	{
		Loggger::fatal("Error creating program");
	}

	for (auto i = 0; i < size; i++)
	{
		const unsigned int shader = src ? Shaders::shaderSource(types[i], srcs[i]) : Shaders::shaderFile(types[i], srcs[i]);
		glAttachShader(program, shader);
		shaders.push_back(shader);
	}

	glLinkProgram(program);
	Utils::checkError();
	Utils::CheckDebugLog();
	glValidateProgram(program);
	Utils::checkError();
	Utils::CheckDebugLog();

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		throw ProgramLinkException(getProgramLog(program), program);
	}
	
	programs.push_back(program);
	return program;
}


const char* Shaders::getProgramLog(const unsigned program)
{
	int length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	std::string message;
	glGetProgramInfoLog(program, length, &length, &message[0]);
	const char* c_message = message.c_str();
	return c_message;
}


const char* Shaders::getShaderLog(const unsigned id)
{
	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

	char* message = static_cast<char*>(_malloca(length * sizeof(char)));
	glGetShaderInfoLog(id, length, nullptr, message);
	return message;
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

Shaders::Program::Program() : binding(20), ID(0)
{
}

Shaders::Program::Program(std::string& vertexPath, std::string& fragmentPath) :	binding(20),
																				vertexPath(vertexPath),
																				fragmentPath(fragmentPath)
{
	Program::ID = loadShaders(false, {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER}, {vertexPath, fragmentPath});
}
void Shaders::Program::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
void Shaders::Program::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::Program::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shaders::Program::setVec3(const std::string& name, glm::vec3 v) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}

void Shaders::Program::setUniform(const std::string& name, UncheckedUniformBuffer buffer)
{
	unsigned binding_ = 0;
	unsigned ubi = 0;

	const auto c_name = name.c_str();
	if(true)
	{
		binding = ++binding;
		binding_ = binding;
		ubi = glGetUniformBlockIndex(ID, c_name);
		Utils::checkError();
		Loggger::debug("Binding uniform %s to program %d with index %d", name.c_str(), ID, ubi);

		binding_map[c_name] = std::make_tuple(binding_, ubi);
	} else
	{
		auto tup = binding_map.at(c_name);
		binding_ = std::get<0, unsigned, unsigned>(tup);
		ubi = std::get<1, unsigned, unsigned>(tup);
		Loggger::fatal("Read binding for %s from map", c_name);
	}

	glUniformBlockBinding(ID, ubi, binding_);
	Utils::checkError();
	buffer.bind(binding_);
}

void Shaders::Program::setUniform(const int binding, UncheckedUniformBuffer buffer) const
{
	buffer.bind(binding);
}


void Shaders::Program::setTexture(const unsigned location, const Texture& texture) const
{
	if(!texture.defined)
	{
		this->setFloat("texture_" + std::to_string(location), texture.substituteValue);
	}
	texture.bind(location);
	Utils::checkError();
}

void Shaders::Program::setTexture(const std::string& name, const Texture& texture)
{
	if (!texture.defined)
	{
		this->setFloat("texture_" + name, texture.substituteValue);
	}
	const unsigned binding_ = ++binding;
	const auto ubi = glGetUniformBlockIndex(ID, name.c_str());
	texture.bind(binding);
	glUniformBlockBinding(ID, ubi, binding_);
	Utils::checkError();
}

void Shaders::Program::use() const
{
	glUseProgram(ID);
	Utils::checkError();
}