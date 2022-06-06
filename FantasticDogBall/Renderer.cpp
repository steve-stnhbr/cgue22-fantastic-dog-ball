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
void Renderer::render(const RenderObject::renderList& objects, Light::Lights lights, float dTime)
{
	Loggger::info("Rendering (%llu):", frameCount);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_FRONT);
	//todo add other lights to all shadow maps
	lights.generateAllShadowMaps(objects);
	glCullFace(GL_BACK);
	glViewport(0, 0, Globals::WINDOW_WIDTH, Globals::WINDOW_HEIGHT);

	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		auto element = *it;
		Loggger::info("\t%s", element.name.c_str());
		element.update(frameCount, dTime);

		// bind program
		auto& prog = element.material->getProgram();
		prog.use();
		
		// bind uniforms here
		camera.bindWithModel(prog, element.transform);
		lights.bind(prog);
		element.draw(prog);
	}

	timeF += .01f;
	frameCount++; 
}