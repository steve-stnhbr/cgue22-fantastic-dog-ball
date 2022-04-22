#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "Utils.h"
#include "UncheckedUniformBuffer.h"

namespace Light
{
	constexpr unsigned NUM_LIGHTS = 1;

	struct Light
	{
		Light() = default;
	};

	struct Directional : Light
	{
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		Directional() = default;
		Directional(glm::vec3 direction,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular);
	};

	struct Point : Light
	{
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		Point() = default;
		Point(glm::vec3 position,
			float constant,
			float linear,
			float quadratic,
			glm::vec3 ambient,
			glm::vec3 diffuse,
			glm::vec3 specular);
	};

	struct Spot : Light
	{
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

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
	};
	
	class Lights
	{
	public:
		std::vector<Point> pLights;
		std::vector<Directional> dLights;
		std::vector<Spot> sLights;
		UncheckedUniformBuffer buffer;

		struct Data
		{
			Point* pLights;
			Directional* dLights;
			Spot* sLights;
		} data = {};

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

	private:
		void update();
	};
}

