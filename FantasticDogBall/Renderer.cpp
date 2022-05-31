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
void Renderer::render(const std::vector<RenderObject>& objects, Light::Lights lights, float dTime)
{
	Loggger::info("Rendering (%llu):", frameCount);

	for (RenderObject element : objects)
	{
		Loggger::info("\t%s", element.name.c_str());
		element.update(frameCount, dTime);

		// bind program
		auto prog = element.material->getProgram();
		prog.use();
		// bind uniforms here
		camera.bindWithModel(prog, element.transform);
		lights.bind(prog);
		element.draw(prog);
		// draw
	}

	timeF += .01f;
	frameCount++; 
}