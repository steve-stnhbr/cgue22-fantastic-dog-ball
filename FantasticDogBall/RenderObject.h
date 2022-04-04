#pragma once

#include <vector>
#include <GL/glew.h>

#include "Vertex.h"

class RenderObject
{
private:
	std::vector<Vertex> vertex_array;
	std::vector<GLuint> index_array;


public:
	RenderObject(std::vector<Vertex> vertex_array, std::vector<GLuint> index_array);
};

