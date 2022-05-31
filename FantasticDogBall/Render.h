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
		std::string fileName;
		unsigned int vboID, eboID;
		std::vector<Vertex> vertex_array;
		std::vector<unsigned int> index_array;

		Render::Mesh() = default;
		Render::Mesh(std::vector<Vertex>, std::vector<unsigned>);

		void createBuffers(unsigned int vaoID) const;
		void bind(Shaders::Program prog);


		static std::vector<Render::Mesh> fromFile(const std::string& path);
		static std::vector<Render::Mesh> fromFile(const std::string& path, const unsigned flags);
	};

	struct Cube: public Mesh
	{
		Cube(float, float, float, float, float, float);
	};

	struct Sphere : public Mesh
	{
		Sphere(float radius, unsigned longitude_segments, unsigned latitude_segments);
	};

	struct Plane : public Mesh {
		Plane(float widht, float depth);
	};

}

