#pragma once
#include <glm/vec3.hpp>
#include <tuple>
#include <vector>

#include "UniformBuffer.h"

namespace Light_
{
	class Point;
	class Directional;
	class Spot;

	struct Values
	{};

	template<typename V>
	class Light
	{
	public:
		V values;
		UncheckedUniformBuffer buffer;

		UncheckedUniformBuffer getBuffer() const;
		const V* getValues() const;

		void update(V*);

	protected:
		Light(V);
	};

	template<typename V>
	class Lights
	{
	public:
		std::vector<Light<V>> elements;
		std::vector<Values> values;

		Lights(std::vector<Light<V>> elements);

		void add(Light<V> e);
	};


	struct DValues : Values
	{
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		DValues() = default;
	};

	class Directional : Light<DValues>
	{
	public:
		Directional();
		Directional(DValues);
		~Directional();
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

		PValues() = default;
	};
	class Point : Light<PValues>
	{
	public:
		Point();
		Point(PValues);
		~Point();
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

		SValues() = default;
	};
	class Spot : Light<SValues>
	{
	public:
		Spot();
		Spot(SValues);
		~Spot();
	};
}
