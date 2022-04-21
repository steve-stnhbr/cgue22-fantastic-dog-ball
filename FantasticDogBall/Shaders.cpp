#include "Shaders.h"

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
	const unsigned int id = glCreateShader(type);
	const char* cSrc = src.c_str();
	glShaderSource(id, 1, &cSrc, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		throw ShaderCompilationException(getShaderLog(id), src.c_str());
	}

	return id;
}

unsigned Shaders::shaderFile(unsigned type, const std::string& src)
{
	fprintf(stdout, "Creating shader(%d) from file %s\n", type, src.c_str());
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

	for (auto i = 0; i < size; i++)
	{
		const unsigned int shader = src ? Shaders::shaderSource(types[i], srcs[i]) : Shaders::shaderFile(types[i], srcs[i]);
		glAttachShader(program, shader);
		shaders.push_back(shader);
	}

	glLinkProgram(program);
	glValidateProgram(program);

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

	char* message = static_cast<char*>(_malloca(length * sizeof(char)));
	glGetProgramInfoLog(program, length, nullptr, message);
	return message;
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

Shaders::Program::Program(std::string vertexPath, std::string fragmentPath)
{
	Program::vertexPath = vertexPath;
	Program::fragmentPath = fragmentPath;

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

void Shaders::Program::setUniform(const std::string& name, UncheckedUniformBuffer buffer)
{
	auto binding_ = buffer.id + binding;
	Loggger::debug("Binding uniform %s to program %d", name.c_str(), ID);
	const auto ubi = glGetUniformBlockIndex(ID, name.c_str());
	buffer.bind(binding_);
	glUniformBlockBinding(ID, ubi, binding_);
	Utils::checkError();
}


void Shaders::Program::setTexture(const unsigned location, const Texture& texture) const
{
	if(!texture.defined)
	{
		this->setFloat("texture" + std::to_string(location), texture.substituteValue);
	}

	/*
	GLuint textureHandle;
	glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);
	glTextureStorage2D(textureHandle, 1, GL_RGBA8, texture.width, texture.height);
	glTextureSubImage2D(textureHandle, 0, 0, 0, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	*/
	glBindTextureUnit(location, texture.glID);
	Utils::checkError();
}

void Shaders::Program::use()
{
	glUseProgram(ID);
	Utils::checkError();
}