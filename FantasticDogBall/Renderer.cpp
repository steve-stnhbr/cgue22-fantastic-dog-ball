#include "Renderer.h"

#include "Utils.h"

unsigned long long frameCount = 0;

Renderer::Renderer()
{
}

void Renderer::render(const std::vector<RenderObject>& objects)
{
	fprintf(stdout, "Rendering (%llu): \n", frameCount);
	for (const RenderObject element : objects)
	{
		fprintf(stdout, "\t%s\n", element.name.c_str());
		// bind program
		(*element.material).getProgram().use();
		Utils::checkError();
		glBindVertexArray(element.vaoID);
		glBindVertexBuffer(0, element.vboID, 0, sizeof(Vertex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.eboID);
		Utils::checkError();
		// draw
		glDrawElements(GL_TRIANGLES, element.mesh.index_array.size(), GL_UNSIGNED_INT, nullptr);
		Utils::checkError();
	}

	frameCount++; 
}