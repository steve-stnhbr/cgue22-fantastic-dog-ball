#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"

namespace Render
{
	// This class represents a mesh with an array of vertecies and the corresponding index array
	struct Mesh
	{
		const std::vector<Vertex> vertex_array;
		const std::vector<unsigned> index_array;
	};

	struct Material{};
	// A material that is defined by constant values for material properties
	struct StaticMaterial : Material
	{
		const glm::vec4 color;
		const float roughness;
		const float transmission;
		const float indexOfRefraction;
		const float metallic;
		const float specularity;
	};

	// at the moment the texture type describes the location of a bitmap
	typedef std::string texture;
	// A material that is defined by different textures allowing for non-static properties
	struct MapMaterial : Material
	{
		const texture color;
		const texture roughness;
		const texture transmission;
		const texture indexOfRefraction;
		const texture metallic;
		const texture specularity;

		const texture normal;
	};

	// at the moment the shader type describes the location of a source file
	typedef std::string shader;
	// A Material that is defined by different shader files allowing for procedural generation of properties
	struct ProceduralMaterial : Material
	{
		const shader color;
		const shader roughness;
		const shader transmission;
		const shader indexOfRefraction;
		const shader metallic;
		const shader specularity;
		
		const shader normal;
	};


}

