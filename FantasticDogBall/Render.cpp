#include "Render.h"

Render::Mesh::Mesh(std::vector<Vertex>& v_, std::vector<unsigned>& i_) : vertex_array(v_), index_array(i_)
{
	using indexType = std::decay_t<decltype(*index_array.begin())>;
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(index_array.size() * sizeof(indexType)), index_array.data(), GL_STATIC_DRAW);
	using vertexType = std::decay_t<decltype(*index_array.begin())>;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(index_array.size() * sizeof(vertexType)), index_array.data(), GL_STATIC_DRAW);
}

unsigned Render::loadProgram(const std::vector<GLenum>& types, const std::vector<std::string>& files)
{
	try
	{
		const unsigned program = Shaders::loadShadersFile(types, files);
		return program;
	}
	catch (std::length_error& e)
	{
		fprintf(stderr, "Impossible");
		glfwTerminate();
		exit(-11);
	}
	catch (Shaders::ShaderCompilationException& e)
	{
		fprintf(stderr, "Shader compile error (%s):%s\n", e.shaderName.c_str(), e.what());
		glfwTerminate();
		exit(-12);
	}
	catch (Shaders::ProgramLinkException& e)
	{
		fprintf(stderr, "Program link error (%d):%s\n", e.program, e.what());
		glfwTerminate();
		exit(-13);
	}
}