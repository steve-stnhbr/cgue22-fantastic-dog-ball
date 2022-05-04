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
	

	Loggger::debug("Created program %u the following shaders: ", program);
	for (auto i = 0; i < srcs.size(); i++) {
		Loggger::debug("%u : %s", types[i], srcs[i].c_str());
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

Shaders::Program::Program() : location(10), binding(20), ID(0)
{
}

Shaders::Program::Program(std::string& vertexPath, std::string& fragmentPath) :	location(10),
																				binding(20),
																				vertexPath(vertexPath),
																				fragmentPath(fragmentPath)
{
	try
	{
		Program::ID = loadShaders(false, { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, { vertexPath, fragmentPath });
	}
	catch (Shaders::ShaderCompilationException& e)
	{
		Utils::checkError();
		Loggger::error("Failed to compile shader (%s): %s", e.shaderName.c_str(), e.what());
		exit(-10);
	}
	catch (Shaders::ProgramLinkException& e)
	{
		Utils::checkError();
		Loggger::error("Failed to link program (%d): %s", e.program, e.what());
		exit(-11);
	}
	catch (std::exception& e)
	{
		Utils::checkError();
		Loggger::error("Failed to link program: %s", e.what());
		exit(-12);
	}
}

Shaders::Program::Program(std::vector<std::string> paths) : binding(20), location(30)
{
	std::vector<GLenum> types;
	for (std::string s : paths) {
		GLenum type = 0;
		const auto index = s.find_last_of(".");
		const auto extension = s.substr(index + 1, s.length() -1);
		Utils::strToLower(extension);
		if (extension == "frag" || extension == "fragment")
			type = GL_FRAGMENT_SHADER;
		else if (extension == "vert" || extension == "vertex")
			type = GL_VERTEX_SHADER;
		else if (extension == "geo" || extension == "geometry")
			type = GL_TESS_CONTROL_SHADER;
		else if (extension == "tes")
			type = GL_TESS_EVALUATION_SHADER;
		else if (extension == "tcs")
			type = GL_TESS_CONTROL_SHADER;
		else {
			Loggger::fatal("Could not read shader type %s from file %s\nValid extensions are .frag, .fragment, .vert, .vertex, .tcs, .tes, .geo, .geometry", extension.c_str(), s.c_str());
			exit(-13);
			//throw new ShaderFileExtensionException("could not read shader type from file", s.c_str(), extension);
		}

		types.push_back(type);
	}
	try
	{
		Program::ID = loadShaders(false, types, paths);
	}
	catch (Shaders::ShaderCompilationException& e)
	{
		Utils::checkError();
		Loggger::error("Failed to compile shader (%s): %s", e.shaderName.c_str(), e.what());
		exit(-10);
	}
	catch (Shaders::ProgramLinkException& e)
	{
		Utils::checkError();
		Loggger::error("Failed to link program (%d): %s", e.program, e.what());
		exit(-11);
	}
	catch (std::exception& e)
	{
		Utils::checkError();
		Loggger::error("Failed to link program: %s", e.what());
		exit(-12);
	}
}

void Shaders::Program::setBool(const std::string& name, const bool value) const
{
	Loggger::debug("Setting %s to %b", name.c_str(), value);
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}
void Shaders::Program::setInt(const std::string& name, const int value) const
{
	Loggger::debug("Setting %s to %i", name.c_str(), value);
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::Program::setFloat(const std::string& name, const float value) const
{
	Loggger::debug("Setting %s to %f", name.c_str(), value);
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shaders::Program::setVec3(const std::string& name, glm::vec3 v) const
{
	Loggger::debug("Setting %s to (%f, %f, %f)", name.c_str(), v.x, v.y, v.z);
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}

void Shaders::Program::setUniform(const std::string& name, UncheckedUniformBuffer buffer)
{
	unsigned binding_ = 0;
	unsigned ubi = 0;

	const auto c_name = name.c_str();
	auto map_val = binding_map.find(c_name);
	if(true)
	{
		binding = ++binding;
		binding_ = binding;
		ubi = glGetUniformBlockIndex(ID, c_name);
		Utils::checkError();
		Loggger::debug("Binding uniform %s to program %d with index %d", c_name, ID, ubi);

		binding_map.insert(std::pair<const char*, std::pair<unsigned, unsigned>>(c_name, std::pair<unsigned, unsigned>(binding_, ubi)));
	} else
	{
		binding_ = map_val->second.first;
		ubi = map_val->second.second;
		Loggger::fatal("Read binding for %s from map: %u", c_name, binding_);
	}

	glUniformBlockBinding(ID, ubi, binding_);
	Utils::checkError();
	buffer.bind(binding_);
}

void Shaders::Program::setUniform(const int binding, UncheckedUniformBuffer buffer) const
{
	buffer.bind(binding);
}


void Shaders::Program::setTexture(const unsigned location, const Texture::Texture& texture) const
{
	if(!texture.defined)
	{
		this->setFloat("texture_" + std::to_string(location), texture.substituteValue);
	}
	texture.bind(location);
	Utils::checkError();
}

void Shaders::Program::setTexture(const std::string& name, const Texture::Texture& texture)
{
	if (!texture.defined)
	{
		this->setInt("s_" + name, 0);
		this->setFloat("value_" + name, texture.substituteValue);
		return;
	}
	this->setInt("s_" + name, 1);
	
	const unsigned location_ = ++location;
	const auto ul = glGetUniformLocation(ID, name.c_str());
	if (ul < 0)
		Loggger::error("The location for %s was not found in shader %u", name.c_str(), ID);
		
	Utils::checkError();
	Loggger::debug("Binding texture %s to location %u", name.c_str(), location);
	glUniform1i(ul, location);
	Utils::checkError();
	texture.bind(location);
	Utils::checkError();
}

void Shaders::Program::use() const
{
	Loggger::debug("Using program %u", ID);
	glUseProgram(ID);
	Utils::checkError();
}

Shaders::ShaderFileExtensionException::ShaderFileExtensionException() : base(""), file(""), extension("")
{
}

Shaders::ShaderFileExtensionException::ShaderFileExtensionException(const char* const message) : base(message)
{
}

Shaders::ShaderFileExtensionException::ShaderFileExtensionException(const char* const message, const char* const file_, const char* const extension_) : base (message), file(file_), extension(extension_)
{
}
