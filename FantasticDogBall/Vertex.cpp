#include "Vertex.h"

#include <glm/glm.hpp>

Vertex::Vertex(const glm::vec3 position) : position(position), normal(0), texture_coordinate(0), color(0)
{}

Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal) : position(position), normal(normal), texture_coordinate(0), color(0)
{}

Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 texture_coordinate): position(position), normal(normal), texture_coordinate(texture_coordinate), color(0)
{}

Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec4 color): position(position), normal(normal), texture_coordinate(0), color(color)
{}
