#include "Renderer.h"
#include "Camera.h"
#include "Utils.h"

unsigned long long frameCount = 0;

Renderer::Renderer()
{
}

/**
 * \brief 
 * \param objects 
 */
void Renderer::render(const std::vector<RenderObject>& objects)
{
	
	fprintf(stdout, "Rendering (%llu): \n", frameCount);
	for (const RenderObject element : objects)
	{
		fprintf(stdout, "\t%s\n", element.name.c_str());
		// bind program
		auto prog = element.material->getProgram();
		prog.use();
		// bind uniforms here
		camera.setData(Camera::Data{glm::mat4(1), glm::mat4(1) });
		prog.setUniform("CameraData", 1, camera.buffer);
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