#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <glm/vec4.hpp>

#include "Shaders.h"
#include "UncheckedUniformBuffer.h"
#include "RenderObject.h"

namespace Light
{

	struct Light
	{
		static Shaders::Program* SHADOW_PROGRAM_2D


		Texture::Texture* shadowMap;
		virtual Texture::Texture* generateShadowMap(const std::vector<RenderObject>& objects) = 0;

		Light() = default;
	};

	struct Directional : Light
	{
		struct Data {
			glm::vec4 direction;

			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;

		} data;

		Directional() = default;
		Directional(glm::vec3 direction,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular);

		Texture::Texture* generateShadowMap(const std::vector<RenderObject>& objects) override;
	};

	struct Point : Light
	{
		Texture::Cubemap* shadowMap;

		struct Data {
			glm::vec4 position;
			glm::vec4 attenuation;
			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;
		} data;

		Point() = default;
		Point(glm::vec3 position,
			float constant,
			float linear,
			float quadratic,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular); 

		Texture::Cubemap* generateShadowMap(const std::vector<RenderObject>& objects) override;
	};

	struct Spot : Light
	{
		struct Data {
			glm::vec4 position;
			glm::vec4 direction;
			glm::vec4 cutoff;
			glm::vec4 attenuation;
			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;
		} data;

		Spot() = default;
		Spot(glm::vec3 position,
			glm::vec3 direction,
			float cutOff,
			float outerCutOff,
			float constant,
			float linear,
			float quadratic,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular);

		Texture::Texture* generateShadowMap(const std::vector<RenderObject>& objects) override;
	};
	
	class Lights
	{
	public:
		std::vector<Point::Data> pLights;
		std::vector<Directional::Data> dLights;
		std::vector<Spot::Data> sLights;
		UncheckedUniformBuffer pBuffer, dBuffer, sBuffer;
		bool finalized;

		Lights();

		void add(Point);
		void add(Directional);
		void add(Spot);

		template <typename T>
		inline unsigned int amount() const
		{
			if (std::is_same_v<T, Point>)
			{
				return pLights.size();
			}
			if (std::is_same_v<T, Directional>)
			{
				return dLights.size();
			}
			if (std::is_same_v<T, Spot>)
			{
				return sLights.size();
			}
			return 0;
		}

		template <typename T>
		inline bool empty() const
		{
			return amount<T>() == 0;
		}

		void bind(Shaders::Program&);

		void finalize();
	};
}

