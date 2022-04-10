#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "Shaders.h"
#include "Vertex.h"

namespace Render
{
	// at the moment the shader type describes the location of a source file
	typedef std::string shader;
	const shader vertexShader = "./VertexShader.glsl";
	const shader colorFragmentShader = "./FragmentShader.glsl";
	const shader textureFragmentShader = "./FragmentShader.glsl";

	unsigned loadProgram(const std::vector<GLenum>& types, const std::vector<std::string>& files);

	// This class represents a mesh with an array of vertecies and the corresponding index array
	struct Mesh
	{
		const std::vector<Vertex> vertex_array;
		const std::vector<unsigned> index_array;
	};
	
	struct Material
	{
		virtual unsigned getVAO() = 0;
		virtual Shaders::Program getProgram() = 0;
		virtual ~Material() = default;
	};
	// A material that is defined by constant values for material properties
	struct StaticMaterial final : public Material
	{
		const glm::vec4 color = {};
		const float roughness = 0;
		const float transmission = 0;
		const float indexOfRefraction = 0;
		const float metallic = 0;
		const float specularity = 0;

		const Shaders::Program program = Shaders::Program(vertexShader, colorFragmentShader);
		const static unsigned VAOi = 0;

		unsigned getVAO() override
		{
			return VAOi;
		}

		Shaders::Program getProgram() override
		{
			return program;
		}
	};

	// at the moment the texture type describes the location of a bitmap
	struct Texture
	{
		std::string filePath;
		int width, height, nrChannels;

		unsigned char* data;
		bool defined;

		Texture(std::string filePath_)
		{
			if(filePath_.empty())
			{
				defined = false;
			} else
			{
				filePath = filePath_;
				data = stbi_load(filePath_.c_str(), &width, &height, &nrChannels, 0);
			}
		}
	};
	// A material that is defined by different textures allowing for non-static properties
	struct TextureMaterial final : public Material
	{
		const Texture color;
		const Texture roughness;
		const Texture transmission;
		const Texture indexOfRefraction;
		const Texture metallic;
		const Texture specularity;

		const Texture normal;

		const Shaders::Program program = Shaders::Program(vertexShader, textureFragmentShader);
		const static unsigned VAOi = 1;

		unsigned getVAO() override
		{
			return VAOi;
		}

		Shaders::Program getProgram() override
		{

			return program;
		}
	};


	// A Material that is defined by different shader files allowing for procedural generation of properties
	struct ProceduralMaterial final : public Material
	{
		const shader shaderFile;

		const Shaders::Program program = Shaders::Program(vertexShader, shaderFile);
		const static unsigned VAOi = 2;

		unsigned getVAO() override
		{
			return VAOi;
		}

		Shaders::Program getProgram() override
		{
			return program;
		}
	};
}

