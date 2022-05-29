#pragma once

#include <glm/glm.hpp>
#include <string>

// This class defines all the different values for a single point on the mesh
class Vertex {
private:

public:
	Vertex() = default;
	explicit Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinate);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec4 color);

	friend bool operator==(const Vertex&, const Vertex&);
	friend Vertex operator+(const Vertex&, const Vertex&);
	friend Vertex operator*(const Vertex&, float i);
	friend Vertex operator/(const Vertex&, float i);

	// defines the position in object-space 
	glm::vec3 position;
	// defines the normal of the surface at the point
	glm::vec3 normal;
	// defines the coordinate of the texture at the point
	// constraint: 0 >= v.x >= 1, 0 >= v.y >= 1
	glm::vec2 texture_coordinate;
	// defines the color at the point
	glm::vec4 color;

	const std::string toString() const;

	static Vertex halfway(Vertex v0, Vertex v1);
};