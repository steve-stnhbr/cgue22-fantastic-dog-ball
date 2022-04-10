#pragma once

#include "RenderObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer();
	GLenum renderMode;
	void render(const std::vector<RenderObject>&);


protected:
	unsigned vaos[3];
	void initVAOs();

private:
	unsigned generateEBO(const std::vector<unsigned int>&);
};
