#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "UncheckedUniformBuffer.h"
#include "Utils.h"

namespace Light
{

	struct Light
	{
		static unsigned SHADOW_MAP_RESOLUTION;
		static unsigned SHADOW_FRAMEBUFFER;
		static Shaders::Program SHADOW_PROGRAM;

		Texture::Texture shadowMap;

		Light();
		Light(bool useShadowMap);

		Texture::Texture generateShadowMap();

		virtual glm::mat4 getLightSpace() = 0;
	};

	struct Directional : Light
	{
		glm::vec4 direction;

		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

		Directional() = default;
		Directional(glm::vec3 direction,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular);

		glm::mat4 getLightSpace() override;
	};

	struct Point : Light
	{
		glm::vec4 position;
		glm::vec4 attenuation;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

		Point() = default;
		Point(glm::vec3 position,
			float constant,
			float linear,
			float quadratic,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular);
		glm::mat4 getLightSpace() override;
	};

	struct Spot : Light
	{
		glm::vec4 position;
		glm::vec4 direction;
		glm::vec4 cutoff;
		glm::vec4 attenuation;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

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
		glm::mat4 getLightSpace() override;
	};
	
	class Lights
	{
	public:
		std::vector<Point> pLights;
		std::vector<Directional> dLights;
		std::vector<Spot> sLights;
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

