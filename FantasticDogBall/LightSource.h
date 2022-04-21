#pragma once
#include <glm/vec3.hpp>
#include <vector>

namespace Light
{
	template<typename V>
	class Lights : public std::vector<V>
	{
	public:
		Lights() = default;
		Lights(std::vector<V>);
	};

	struct Light
	{};

	struct Directional : Light
	{
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
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
	};
}

