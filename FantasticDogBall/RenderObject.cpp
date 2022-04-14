#include "RenderObject.h"

#include "Utils.h"

RenderObject::RenderObject(Render::Mesh mesh_, Render::Material* material_, const std::string& name_): mesh(mesh_), material(material_), name(name_)
{
	buildVAO();
}

void RenderObject::buildVAO() const
{
	// generate and bind vao here
	Utils::checkError();
	// glBindVertexArray(vaoID);
	Utils::checkError();

	glCreateBuffers(1, (GLuint*) &vboID);
	glNamedBufferStorage(vboID, mesh.vertex_array.size() * sizeof(Vertex), mesh.vertex_array.data(), GL_DYNAMIC_STORAGE_BIT);
	Utils::checkError();

	glCreateBuffers(1, (GLuint*)&eboID);
	glNamedBufferStorage(eboID, mesh.index_array.size() * sizeof(unsigned), mesh.index_array.data(), GL_DYNAMIC_STORAGE_BIT);
	Utils::checkError();

	using vertexType = std::decay_t<decltype(*mesh.vertex_array.begin())>;
	using indexType = std::decay_t<decltype(*mesh.index_array.begin())>;

	glCreateVertexArrays(1, (GLuint*)&vaoID);
	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(vertexType));
	glVertexArrayElementBuffer(vaoID, eboID);
	
	material->assignVertexAttributes(vaoID);
}
