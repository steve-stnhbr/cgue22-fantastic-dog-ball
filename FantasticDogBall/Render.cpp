#include "Render.h"

#include <GLFW/glfw3.h>

Render::Mesh::Mesh(std::vector<Vertex> v_, std::vector<unsigned> i_) : vertex_array(std::move(v_)), index_array(
	                                                                       std::move(i_))
{
}

Render::Cube::Cube(float centerX, float centerY, float centerZ, float width, float height, float depth) : Mesh({
			Vertex {
				{
					-1.0f, -1.0f, -1.0f
				},
				{
					0, 0, 0
				},
				{
					0.0f, 1.0f, 0.0f, 1.0f
				},
			},Vertex {
				{
					 1.0f, -1.0f, -1.0f
				},
				{
					0, 0, 0
				},
				{
					1.0f, 0.5f, 0.0f, 1.0f
				},
			},Vertex {
				{
					1.0f, 1.0f, -1.0f
				},
				{
					0, 0, 0
				},
				{
					1.0f, 0.0f, 0.0f, 1.0f
				},
			},Vertex {
				{
					-1.0f, 1.0f, -1.0f
				},
				{
					0, 0, 0
				},
				{
					0.0f, 0.0f, 1.0f, 1.0f
				},
			},Vertex {
				{
					-1.0f, -1.0f, 1.0f
				},
				{
					0, 0, 0
				},
				{
					0.0f, 1.0f, 0.0f, 1.0f
				},
			},Vertex {
				{
					  1.0f, -1.0f, 1.0f
				},
				{
					0, 0, 0
				},
				{
					 1.0f, 0.5f, 0.0f, 1.0f
				},
			},Vertex {
				{
					1.0f, 1.0f, 1.0f
				},
				{
					0, 0, 0
				},
				{
					1.0f, 0.0f, 0.0f, 1.0f
				},
			},Vertex {
				{
					-1.0f, 1.0f, 1.0f
				},
				{
					0, 0, 0
				},
				{
					1.0f, 0.0f, 1.0f, 1.0f
				},
			},
	}, {
			5, 4, 0, 1, 5, 0, 6, 5, 1, 2, 6, 1,
			7, 6, 2, 3, 7, 2, 4, 7, 3, 0, 4, 3,
			6, 7, 4, 5, 6, 4, 1, 0, 3, 2, 1, 3
	})
{
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
		Loggger::error("Impossible");
		glfwTerminate();
		exit(-11);
	}
	catch (Shaders::ShaderCompilationException& e)
	{
		Loggger::error("Shader compile error (%s):%s\n", e.shaderName.c_str(), e.what());
		glfwTerminate();
		exit(-12);
	}
	catch (Shaders::ProgramLinkException& e)
	{
		Utils::checkError();
		Loggger::error("Program link error (%d):%s\n", e.program, e.what());
		glfwTerminate();
		exit(-13);
	}
}