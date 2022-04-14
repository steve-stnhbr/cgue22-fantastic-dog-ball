#pragma once

#include "RenderObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class Renderer
{
public:
	Renderer();
	GLenum renderMode;
	Camera camera;

	void render(const std::vector<RenderObject>&);
};
