#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "Shaders.h"
#include "Utils.h"
#include "Vertex.h"
#include "UniformBuffer.h"

namespace Render
{

	unsigned loadProgram(const std::vector<GLenum>& types, const std::vector<std::string>& files);

	// This class represents a mesh with an array of vertecies and the corresponding index array
	struct Mesh
	{
		const std::vector<Vertex> vertex_array;
		const std::vector<unsigned int> index_array;

		Render::Mesh(std::vector<Vertex>, std::vector<unsigned>);
	};

	struct Cube: public Mesh
	{
		Cube(float, float, float, float, float, float);
	};

}

