#include "Render.h"

#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>

Render::Mesh::Mesh(std::vector<Vertex> v_, std::vector<unsigned> i_) : vertex_array(std::move(v_)), index_array(
	                                                                       std::move(i_))
{
}

Render::Mesh Render::Mesh::fromFile(std::string path)
{

}

Render::Cube::Cube(float centerX, float centerY, float centerZ, float width, float height, float depth) : Mesh({
	                                                                                                               // front
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               // back
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //right
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //left
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //top
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //bottom
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {1,1},
	                                                                                                               },
                                                                                                               }, {
	                                                                                                               // front
	                                                                                                               0, 1, 2,
	                                                                                                               2, 3, 0,
	                                                                                                               // back
	                                                                                                               4, 5, 6,
	                                                                                                               6, 7, 4,
	                                                                                                               // right
	                                                                                                               8, 9, 10,
	                                                                                                               10, 11, 8,
	                                                                                                               // left
	                                                                                                               12, 13, 14,
	                                                                                                               14, 15, 12,
	                                                                                                               // top
	                                                                                                               16, 17, 18,
	                                                                                                               18, 19, 16,
	                                                                                                               // bottom
	                                                                                                               20, 21, 22,
	                                                                                                               22, 23, 20
                                                                                                               })
{
}

Render::Sphere::Sphere(float radius, unsigned longitude_segments, unsigned latitude_segments) : Mesh()
{
	std::vector<Vertex> vertecies;
	std::vector<unsigned> indices;

	vertecies.push_back({
		glm::vec3(0.0f, radius, 0.0f),
		glm::vec3(0.0f, radius, 0.0f),
		glm::vec2(0, 0)
		});
	vertecies.push_back({
			glm::vec3(0.0f, -radius, 0.0f),
			glm::vec3(0.0f, -radius, 0.0f),
			glm::vec2(1, 1)
	});

	// first and last ring
	for (uint32_t j = 0; j < longitude_segments; j++) {
		indices.push_back(0);
		indices.push_back(j == longitude_segments - 1 ? 2 : (j + 3));
		indices.push_back(2 + j);

		indices.push_back(2 + (latitude_segments - 2) * longitude_segments + j);
		indices.push_back(j == longitude_segments - 1 ? 2 + (latitude_segments - 2) * longitude_segments : 2 + (latitude_segments - 2) * longitude_segments + j + 1);
		indices.push_back(1);
	}

	// vertices and rings
	for (uint32_t i = 1; i < latitude_segments; i++) {
		const float verticalAngle = float(i) * glm::pi<float>() / float(latitude_segments);
		for (uint32_t j = 0; j < longitude_segments; j++) {
			const float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(longitude_segments);
			glm::vec3 position = glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			);
			auto u = (0.5 + atan2(position.x, position.z) / (2 * glm::pi<double>()));
			auto v = (0.5 - asin(position.y) / glm::pi<double>());

			vertecies.push_back({
				position,
				glm::normalize(position),
				{u, v}
				});
			

			if (i == 1) continue;

			indices.push_back(2 + (i - 1) * longitude_segments + j);
			indices.push_back(j == longitude_segments - 1 ? 2 + (i - 2) * longitude_segments : 2 + (i - 2) * longitude_segments + j + 1);
			indices.push_back(j == longitude_segments - 1 ? 2 + (i - 1) * longitude_segments : 2 + (i - 1) * longitude_segments + j + 1);

			indices.push_back(j == longitude_segments - 1 ? 2 + (i - 2) * longitude_segments : 2 + (i - 2) * longitude_segments + j + 1);
			indices.push_back(2 + (i - 1) * longitude_segments + j);
			indices.push_back(2 + (i - 2) * longitude_segments + j);
		}
	}

	vertex_array = vertecies;
	index_array = indices;
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
