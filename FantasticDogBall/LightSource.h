#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

#include "UncheckedUniformBuffer.h"
#include "Utils.h"
#include "RenderObject.h"

namespace Light
{

	struct Light
	{
		static unsigned SHADOW_MAP_RESOLUTION;
		static unsigned SHADOW_FRAMEBUFFER;
		static Shaders::Program SHADOW_PROGRAM;
		static void initProgram();

		bool castShadow;
		Texture::Texture shadowMap;

		Light();
		Light(bool useShadowMap);

		//Texture::Texture generateShadowMap(const std::vector<RenderObject>&);

		Texture::Texture generateShadowMap2D(const std::vector<RenderObject>&);

		virtual void generateShadowMap(const std::vector<RenderObject>&) = 0;
		virtual glm::mat4 getLightSpace() const = 0;
	};

	struct Directional : Light
	{
		struct Data {
			glm::vec4 direction;

			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;

			Data() = default;
			Data(glm::vec4 direction,
				glm::vec4 ambient,
				glm::vec4 diffuse,
				glm::vec4 specular);
		} data;

		Directional() = default;
		Directional(glm::vec3 direction,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular,
			bool castShadow = false);

		glm::mat4 getLightSpace() const override;
		void generateShadowMap(const std::vector<RenderObject>&) override;
	};

	struct Point : Light
	{
		struct Data {
			glm::vec4 position;
			glm::vec4 attenuation;
			glm::vec4 ambient;
			glm::vec4 diffuse;
			glm::vec4 specular;
			
			Data() = default;
			Data(glm::vec4 position,
				glm::vec4 attenuation,
				glm::vec4 ambient,
				glm::vec4 diffuse,
				glm::vec4 specular);
		} data;

		Point() = default;
		Point(glm::vec3 position,
			float constant,
			float linear,
			float quadratic,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular,
			bool castShadow = false);
		glm::mat4 getLightSpace() const override;
		void generateShadowMap(const std::vector<RenderObject>&) override;
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

			Data() = default;
			Data(glm::vec4 position,
				glm::vec4 direction,
				glm::vec4 cutoff,
				glm::vec4 attenuation,
				glm::vec4 ambient,
				glm::vec4 diffuse,
				glm::vec4 specular);
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
			glm::vec3 specular,
			bool castShadow = false);
		glm::mat4 getLightSpace() const override;
		void generateShadowMap(const std::vector<RenderObject>&) override;
	};
	
	class Lights
	{
	public:
		std::vector<Light*> allLights;
		std::vector<Point> pLights;
		std::vector<Directional> dLights;
		std::vector<Spot> sLights;
		std::vector<Point::Data> pData;
		std::vector<Directional::Data> dData;
		std::vector<Spot::Data> sData;
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

		void generateAllShadowMaps(const std::vector<RenderObject>&);

		void finalize();
	};
}

