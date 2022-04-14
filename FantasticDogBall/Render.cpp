#include "Render.h"

Render::Mesh::Mesh(std::vector<Vertex> v_, std::vector<unsigned> i_) : vertex_array(std::move(v_)), index_array(
	                                                                       std::move(i_))
{
}

Render::Cube::Cube(float centerX, float centerY, float centerZ, float width, float height, float depth) : Mesh({
			Vertex {
				{
					centerX + width/2, centerY + height/2, centerZ +depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX - width/2, centerY + height/2, centerZ +depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX + width/2, centerY - height/2, centerZ +depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX - width/2, centerY - height/2, centerZ +depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX + width/2, centerY + height/2, centerZ -depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX - width/2, centerY + height/2, centerZ -depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX + width/2, centerY - height/2, centerZ -depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},Vertex {
				{
					centerX - width/2, centerY - height/2, centerZ -depth/2
				},
				{
					0, 0, 0
				},
				{
					1.0f, .0f, .0f, 1.0f
				},
			},
	}, {
		0,1,2,1,2,3,3,4,5
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