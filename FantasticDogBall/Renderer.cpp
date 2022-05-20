#include "Renderer.h"
#include "Camera.h"
#include "LightSource.h"
#include "Loggger.h"
#include "Utils.h"

#include <glm/gtx/string_cast.hpp>

unsigned long long frameCount = 0;
float timeF = 0;

Renderer::Renderer()
{
}

/**
 * \brief 
 * \param objects 
 */
void Renderer::render(const std::vector<RenderObject>& objects, Light::Lights lights)
{
	Loggger::info("Rendering (%llu):", frameCount);
	std::vector<Vertex> allMeshes;

	for (RenderObject e : objects) {
		allMeshes.insert(allMeshes.end(),
			e.mesh.vertex_array.begin(),
			e.mesh.vertex_array.end());
	}


	//Texture::Texture shadowMap = lights.dLights[0].generateShadowMap(objects);s
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT);

	for (RenderObject element : objects)
	{
		element.update();

		Loggger::info("\t%s", element.name.c_str());

		// bind program
		auto prog = element.material->getProgram();
		prog.use();

		//prog.setTexture("shadowMap", shadowMap);

		// bind uniforms here
		camera.bindWithModel(prog, element.transform);
		lights.bind(prog);
		element.material->bind(prog);
		Utils::checkError();
		glBindVertexArray(element.vaoID);
		glBindVertexBuffer(0, element.vboID, 0, sizeof(Vertex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element.eboID);
		Utils::checkError();
		// draw
		glDrawElements(GL_TRIANGLES, element.mesh.index_array.size(), GL_UNSIGNED_INT, nullptr);
		Utils::checkError();
		glUseProgram(0);
	}

	timeF += .01f;
	frameCount++; 
}