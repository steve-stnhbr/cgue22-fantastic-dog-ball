#include "Renderer.h"
#include "Camera.h"
#include "Loggger.h"
#include "Utils.h"

unsigned long long frameCount = 0;
float timeF = 0;

Renderer::Renderer()
{
}

/**
 * \brief 
 * \param objects 
 */
void Renderer::render(const std::vector<RenderObject>& objects)
{
	Loggger::info("Rendering (%llu):", frameCount);
	for (RenderObject element : objects)
	{
		Loggger::info("\t%s", element.name.c_str());

		element.rotate(0, timeF, 0);

		// bind program
		auto prog = element.material->getProgram();
		prog.use();
		// bind uniforms here
		camera.bindWithModel(prog, element.transform);
		element.material->bind(prog);
		Utils::checkError();
		glBindVertexArray(element.vaoID);
		glBindVertexBuffer(0, element.vboID, 0, sizeof(Vertex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.eboID);
		Utils::checkError();
		// draw
		glDrawElements(GL_TRIANGLES, element.mesh.index_array.size(), GL_UNSIGNED_INT, nullptr);
		Utils::checkError();
	}

	timeF += .1f;
	frameCount++; 
}