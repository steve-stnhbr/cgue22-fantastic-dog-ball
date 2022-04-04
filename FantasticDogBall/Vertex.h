#pragma once

#include <glm/glm.hpp>

class Vertex {
private:

public:

	explicit Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinate);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec4 color);

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coordinate;
	glm::vec4 color;
};