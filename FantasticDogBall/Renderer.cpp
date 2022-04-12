#include "Renderer.h"

unsigned long long frameCount = 0;

Renderer::Renderer()
{
	renderMode = GL_TRIANGLES;
	initVAOs();
}

void Renderer::render(const std::vector<RenderObject>& objects)
{
	fprintf(stdout, "Rendering (%llu): \n", frameCount);

	for (const RenderObject element : objects)
	{
		fprintf(stdout, "\t%s\n", element.name.c_str());
		// bind program
		(*element.material).getProgram().use();
		// bind vao
		glBindVertexArray(element.material->getVAO());
		// bind ebo (index array)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.mesh.eboID);
		// bind vbo (vertex array)
		glBindBuffer(GL_ARRAY_BUFFER, element.mesh.vboID);
		// draw
		glDrawElements(renderMode, element.mesh.vertex_array.size(), GL_UNSIGNED_INT, 0);
	}

	frameCount++;
}

void Renderer::initVAOs()
{
	fprintf(stdout, "Initializing VAOs\n");
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
	glBindVertexArray(vaos[Render::TextureMaterial::VAOi]);
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
	glBindVertexArray(vaos[Render::ProceduralMaterial::VAOi]);
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(3));
	glEnableVertexAttribArray(1);



	glBindVertexArray(0);
}

