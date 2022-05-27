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

Vertex Vertex::halfway(Vertex v0, Vertex v1)
{
	return (v0 + v1) / 2;
}

Vertex operator/(const Vertex& v, float i)
{
	return Vertex(v.position / i, v.normal / i, v.texture_coordinate / i);
}

bool operator==(const Vertex& v0, const Vertex& v1)
{
	return v0.position == v1.position
		&& v0.normal == v1.normal
		&& v0.color == v1.color
		&& v0.texture_coordinate == v1.texture_coordinate;
}

Vertex operator+(const Vertex& v0, const Vertex& v1)
{
	return Vertex(v0.position + v1.position, v0.normal + v1.normal, v0.texture_coordinate + v1.texture_coordinate);
}

Vertex operator*(const Vertex& v, float i)
{
	return Vertex(v.position * glm::vec3(i), v.normal * glm::vec3(i), v.texture_coordinate * glm::vec2(i));
}
