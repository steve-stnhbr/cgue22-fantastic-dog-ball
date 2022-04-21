#pragma once

#include "RenderObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "LightSource.h"

class Renderer
{
public:
	Renderer();
	GLenum renderMode;
	Camera camera;

	void render(const std::vector<RenderObject>&, const Light::Lights<Light::Point>& pLights,
		const Light::Lights<Light::Directional>& dLights,
		const Light::Lights<Light::Spot>& sLights);
};
