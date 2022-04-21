#pragma once
#include <glm/vec3.hpp>

#include "UniformBuffer.h"

namespace Light
{
	struct Values
	{
		Values() = delete;
	};

	template<typename V>
	class Light
	{
	public:
		V values;
		UniformBuffer<V> buffer;

		virtual UniformBuffer<V> getBuffer() const = 0;
		const V* getValues() const;

		void update(V);
	protected:
		Light(V);
	};


	struct DValues : Values
	{
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	class Directional : Light<DValues>
	{
	};

	struct PValues : Values
	{
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	class Point : Light<PValues>
	{
	};

	struct SValues : Values
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
	};
	class Spot : Light<SValues>
	{
	};
}
