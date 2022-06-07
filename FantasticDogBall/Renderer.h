#pragma once

#include "RenderObject.h"
#include <GL/glew.h>

#include "Camera.h"
#include "LightSource.h"

class Renderer
{
public:
	Renderer();
	GLenum renderMode;
	Camera camera;

	void render(const RenderObject::renderList&, Light::Lights lights, float dTime);
};
