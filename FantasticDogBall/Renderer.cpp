#include "Renderer.h"
#include "Camera.h"
#include "Loggger.h"
#include "Utils.h"

unsigned long long frameCount = 0;
float time = 0;

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
	for (const RenderObject element : objects)
	{
		Loggger::info("\t%s", element.name.c_str());
		// bind program
		auto prog = element.material->getProgram();
		prog.use();
		// bind uniforms here
		camera.bindCamera(prog, glm::rotate(glm::mat4(1), time, {0, 1, 0}));
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

	time += .1f;
	frameCount++; 
}