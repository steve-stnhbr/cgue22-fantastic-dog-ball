#include "Renderer.h"


Renderer::Renderer()
{
	renderMode = GL_TRIANGLES;
	initVAOs();
}

void Renderer::render(const std::vector<RenderObject>& objects)
{
	for (const RenderObject element : objects)
	{
		fprintf(stdout, "Rendering object %s", element.name.c_str());
		(*element.material).getProgram().use();
		glBindVertexArray((*element.material).getVAO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, generateEBO(element.mesh.index_array));
		glDrawElements(renderMode, static_cast<GLsizei>(element.mesh.index_array.size()), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::initVAOs()
{
	constexpr auto vertexSize = 2 * sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec4);
	glCreateVertexArrays(3, Renderer::vaos);

	// initialize color VAO
	glBindVertexArray(vaos[Render::StaticMaterial::VAOi]);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(3));
	glEnableVertexAttribArray(1);
	// color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(8));
	glEnableVertexAttribArray(2);

	// initialize texture VAO
	glBindVertexArray(vaos[Render::StaticMaterial::VAOi]);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(3));
	glEnableVertexAttribArray(1);
	// textureCoordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(6));
	glEnableVertexAttribArray(2);

	// initialize texture VAO
	glBindVertexArray(vaos[Render::StaticMaterial::VAOi]);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(3));
	glEnableVertexAttribArray(1);
}


unsigned Renderer::generateEBO(const std::vector<unsigned>& indices)
{
	using T = std::decay_t<decltype(*indices.begin())>;
	unsigned ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(T)), indices.data(), GL_STATIC_DRAW);
	return ebo;
}
