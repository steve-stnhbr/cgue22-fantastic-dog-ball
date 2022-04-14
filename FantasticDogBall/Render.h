#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "Shaders.h"
#include "Utils.h"
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
		const std::vector<unsigned int> index_array;

		Render::Mesh(std::vector<Vertex>, std::vector<unsigned>);
	};
	
	struct Material
	{
		virtual unsigned getVAO() = 0;
		virtual Shaders::Program getProgram() = 0;
		virtual ~Material() = default;
		virtual void assignVertexAttributes(unsigned vao) = 0;
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

		void assignVertexAttributes(unsigned vao) override
		{
			// position
			// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
			// glEnableVertexAttribArray(0);
			// normal
			// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(3));
			// glEnableVertexAttribArray(1);
			// color
			// glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(8));
			// glEnableVertexAttribArray(2);

			// --- postition ---
			glEnableVertexArrayAttrib(vao, 0);
			Utils::checkError();
			glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 0, 0);
			Utils::checkError();
			// --- normal ---
			glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3);
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 1, 0);
			Utils::checkError();
			glEnableVertexArrayAttrib(vao, 1);
			Utils::checkError();
			// --- color ---
			glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, 8);
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 2, 0);
			Utils::checkError();
			glEnableVertexArrayAttrib(vao, 2);
			Utils::checkError();
		}
	};

	// A material that is defined by different textures allowing for non-static properties
	struct TextureMaterial final : public Material
	{
		const Shaders::Texture color;
		const Shaders::Texture roughness;
		const Shaders::Texture transmission;
		const Shaders::Texture indexOfRefraction;
		const Shaders::Texture metallic;
		const Shaders::Texture specularity;

		const Shaders::Texture normal;

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

