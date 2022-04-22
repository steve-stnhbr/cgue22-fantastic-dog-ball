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
	// at the moment the shader type describes the location of a source file
	typedef std::string shader;
	const shader vertexShader = "./VertexShader.vert";
	const shader colorFragmentShader = "./FragmentShader_static.frag";
	const shader textureFragmentShader = "./FragmentShader.glsl";

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

	struct Material
	{
		virtual Shaders::Program getProgram() = 0;
		virtual ~Material() = default;
		// virtual UncheckedUniformBuffer getBuffer() = 0;
		virtual void* getValues() = 0;
		virtual void bind(Shaders::Program) = 0;
		/*
		 * This function is responsible for assigning streams of vertex-attributes
		 */
		virtual void assignVertexAttributes(unsigned vao) = 0;
	};
	// A material that is defined by constant values for material properties
	struct StaticMaterial final : public Material
	{
		struct Values
		{
			glm::vec4 color = {1.0f, 1.0f, 0.0f, 1.0f};
			float diffuse;
			float specular;
			float shininess;
		};

		Values vals;

		Shaders::Program program;
		UncheckedUniformBuffer buffer;

		StaticMaterial(): program(Utils::loadProgram(vertexShader, colorFragmentShader))
		{
			createBuffer();
		}

		StaticMaterial(
			const glm::vec4 color_,
			const float diffuse_,
			const float specular_,
			const float shininess_):
			vals({color_, diffuse_, specular_, shininess_}),
			program(Utils::loadProgram(vertexShader, colorFragmentShader)                                                                                                                                                                                                                                                  )
		{
			createBuffer();
			Utils::checkError();
			program.setUniform("Material", buffer);
			Utils::checkError();
		}

		Shaders::Program getProgram() override
		{
			return program;
		}

		UncheckedUniformBuffer getBuffer() const
		{
			return buffer;
		}

		void* getValues() override
		{
			return &vals;
		}

		void createBuffer()
		{
			buffer.create(sizeof(Values));
		}

		void bind(Shaders::Program p) override
		{
			buffer.update(sizeof(Values), &vals);
			p.setUniform("Material", buffer);
		}

		/**
		 * This function is responsible for assigning streams of vertex-attributes
		 * location = 0, binding = 0: postition
		 * location = 1, binding = 0: normal
		 * location = 2, binding = 0: color
		 */
		void assignVertexAttributes(unsigned vao) override
		{
			// --- postition ---
			glEnableVertexArrayAttrib(vao, 0);
			Utils::checkError();
			glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 0, 0);
			Utils::checkError();
			// --- normal ---
			glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 1, 0);
			Utils::checkError();
			glEnableVertexArrayAttrib(vao, 1);
			Utils::checkError();
			// --- color ---
			glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
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
		const Texture color;
		const Texture normal;
		const Texture roughness;
		const Texture metallic;
		const Texture specularity;

		const Shaders::Program program = Shaders::Program(vertexShader, textureFragmentShader);

		Shaders::Program getProgram() override
		{
			program.setTexture(0, color);
			program.setTexture(1, normal);
			program.setTexture(2, metallic);
			program.setTexture(3, specularity);
			return program;
		}

		/**
		 * This function is responsible for assigning streams of vertex-attributes
		 * location = 0, binding = 0: postition
		 * location = 1, binding = 0: normal
		 * location = 2, binding = 0: texture_coordinate
		 */
		void assignVertexAttributes(unsigned vao) override
		{
			// --- postition ---
			glEnableVertexArrayAttrib(vao, 0);
			Utils::checkError();
			glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 0, 0);
			Utils::checkError();
			// --- normal ---
			glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 1, 0);
			Utils::checkError();
			glEnableVertexArrayAttrib(vao, 1);
			Utils::checkError();
			// --- color ---
			glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coordinate));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 2, 0);
			Utils::checkError();
			glEnableVertexArrayAttrib(vao, 2);
			Utils::checkError();
		}
	};


	// A Material that is defined by different shader files allowing for procedural generation of properties
	struct ProceduralMaterial final : public Material
	{
		const shader shaderFile;

		const Shaders::Program program = Shaders::Program(vertexShader, shaderFile);

		Shaders::Program getProgram() override
		{
			return program;
		}

		/**
		 * This function is responsible for assigning streams of vertex-attributes
		 * location = 0, binding = 0: postition
		 * location = 1, binding = 0: normal
		 */
		void assignVertexAttributes(unsigned vao) override
		{
			// --- postition ---
			glEnableVertexArrayAttrib(vao, 0);
			Utils::checkError();
			glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 0, 0);
			Utils::checkError();
			// --- normal ---
			glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
			Utils::checkError();
			glVertexArrayAttribBinding(vao, 1, 0);
			Utils::checkError();
			glEnableVertexArrayAttrib(vao, 1);
			Utils::checkError();
		}
	};
}

