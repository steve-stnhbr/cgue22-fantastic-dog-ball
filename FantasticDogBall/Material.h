#pragma once
#include "UncheckedUniformBuffer.h"
#include "Shaders.h"
#include "Utils.h"
#include "Vertex.h"

namespace Material
{
	const std::string vertexShader = "./VertexShader.vert";
	const std::string colorFragmentShader = "./static.frag";
	const std::string textureFragmentShader = "./texture.frag";

	struct Material
	{
		virtual Shaders::Program& getProgram() = 0;
		virtual ~Material() = default;
		virtual void bind(Shaders::Program&) = 0;
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
			glm::vec4 color = { 1.0f, 1.0f, 0.0f, 1.0f };
			glm::vec4 data;

			Values() = default;
			Values(glm::vec4 color_, float diffuse_, float specular_, float shininess_) : color(color_), data(diffuse_, specular_, shininess_, 0) {}
		};

		Values vals;

		static Shaders::Program program;
		UncheckedUniformBuffer buffer;

		StaticMaterial();

		StaticMaterial(const glm::vec4 color_,
			const float diffuse_,
			const float specular_,
			const float shininess_);

		Shaders::Program& getProgram() override;
		void assignVertexAttributes(unsigned vao) override;
		void bind(Shaders::Program&) override;
		void createBuffer();

		static void initProgram();
	};

	// A material that is defined by different textures allowing for non-static properties
	struct TextureMaterial final : public Material
	{
		Texture::Texture color;
		Texture::Texture normal;
		Texture::Texture diffuse;
		Texture::Texture specular;
		float shininess;
		Texture::Texture reflectiveness;

		static Shaders::Program program;

		Shaders::Program& getProgram() override;
		void assignVertexAttributes(unsigned vao) override;
		void bind(Shaders::Program&) override;

		static void initProgram();
	};


	// A Material that is defined by different shader files allowing for procedural generation of properties
	struct ProceduralMaterial final : public Material
	{
		static size_t numPointLights, numDirLights, numSpotLights;
		const std::string shaderFile;

		Shaders::Program program = Shaders::Program({ vertexShader, shaderFile });

		Shaders::Program& getProgram() override;
		void assignVertexAttributes(unsigned vao) override;
		void bind(Shaders::Program&) override; //todo
	};

	static void initPrograms()
	{
		StaticMaterial::initProgram();
		TextureMaterial::initProgram();
	}

}
