#include "RenderObject.h"

#include <vector>

#include "Vertex.h"

RenderObject::RenderObject(std::vector<Vertex> vertex_array, std::vector<GLuint> index_array): vertex_array(std::move(vertex_array)), index_array(std::move(index_array))
{
}